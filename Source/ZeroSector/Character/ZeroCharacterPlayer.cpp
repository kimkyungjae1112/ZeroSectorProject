// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterPlayer.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Data/ZeroInputConfig.h"
#include "Data/ZeroPlayerCameraData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Interface/ZeroDialogueInterface.h"
#include "UI/ZeroOperationWidget.h"
#include "UI/ZeroFadeInAndOutWidget.h"
#include "UI/ZeroProvisoWidget.h"
#include "UI/ZeroGetProvisoWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Weapon/ZeroWeaponRifle.h"
#include "Weapon/ZeroWeaponShotgun.h"
#include "Weapon/ZeroWeaponPistol.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "Gimmick/ZeroOperationBoard.h"
#include "ZeroSector.h"

AZeroCharacterPlayer::AZeroCharacterPlayer() : DetectDistance(800.f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UZeroInputConfig> InputConfigRef(TEXT("/Script/ZeroSector.ZeroInputConfig'/Game/Data/Input/DataAsset/DA_InputConfig.DA_InputConfig'"));
	if (InputConfigRef.Object)
	{
		InputConfig = InputConfigRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UZeroPlayerCameraData> CameraDataRef(TEXT("/Script/ZeroSector.ZeroPlayerCameraData'/Game/Data/Camera/DA_CameraData.DA_CameraData'"));
	if (CameraDataRef.Object)
	{
		CameraData = CameraDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	RifleMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle Mesh Component"));
	RifleMeshComp->SetupAttachment(GetMesh(), TEXT("weapon_rifle"));

	PistolMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Pistol Mesh Component"));
	PistolMeshComp ->SetupAttachment(GetMesh(), TEXT("weapon_Pistol"));

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(GetMesh());
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetRelativeLocation(CameraData->CommonCameraVector);
	CameraComp->SetRelativeRotation(CameraData->CommonCameraRotator);

	ChangeInputMap.Add(EDaySequence::EAfternoon, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputAfternoonMode)));
	ChangeInputMap.Add(EDaySequence::ENight, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputNightMode)));

	CurrentWeaponType = EWeaponType::EPistol;
	TeamId = FGenericTeamId(0);
}

void AZeroCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	InteractBeam();
}

void AZeroCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputConfig->IA_Move, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(InputConfig->IA_Look, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::DialogueInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::OperationBoardInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ProvisoInteract);
	EnhancedInputComponent->BindAction(InputConfig->IA_Fire, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Fire);
	EnhancedInputComponent->BindAction(InputConfig->IA_Aiming, ETriggerEvent::Started, this, &AZeroCharacterPlayer::Aiming);
	EnhancedInputComponent->BindAction(InputConfig->IA_Aiming, ETriggerEvent::Completed, this, &AZeroCharacterPlayer::UnAiming);
	EnhancedInputComponent->BindAction(InputConfig->IA_ChangeWeapon, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ChangeWeapon);
}

FGenericTeamId AZeroCharacterPlayer::GetGenericTeamId() const
{
	return TeamId;
}

void AZeroCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetInputAfternoonMode();
}

APlayerController* AZeroCharacterPlayer::GetPlayerController() const
{
	return CastChecked<APlayerController>(GetController());
}

void AZeroCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, InputValue.X);
	AddMovementInput(RightVector, InputValue.Y);
}

void AZeroCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	AddControllerPitchInput(-InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AZeroCharacterPlayer::Fire()
{
	Weapons[CurrentWeaponType]->Fire();
	ZE_LOG(LogZeroSector, Display, TEXT("Fire"));
}

void AZeroCharacterPlayer::Aiming()
{
	bIsAiming = true;
}

void AZeroCharacterPlayer::UnAiming()
{
	bIsAiming = false;
}

void AZeroCharacterPlayer::ChangeWeapon()
{
	if (ChoicedWeapon == EWeaponType::ERifle)
	{
		if (CurrentWeaponType == EWeaponType::EPistol) SetRifle();
		else SetPistol();
	}
	else if (ChoicedWeapon == EWeaponType::EShotgun)
	{
		if (CurrentWeaponType == EWeaponType::EPistol) SetShotgun();
		else SetPistol();
	}
}

void AZeroCharacterPlayer::DialogueInteract()
{
	if (DialogueInterface)
	{
		DialogueInterface->StartDialogue();
		FOnFinishedDialogue OnFinishedDialogue;
		OnFinishedDialogue.BindLambda([&]()
			{
				SetDefaultMovement();
			});
		DialogueInterface->SetupFinishedDialogueDelegate(OnFinishedDialogue);

		SetDialogueMovement();
	}
}

void AZeroCharacterPlayer::ProvisoInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("Proviso")) == false) return;

	UZeroGetProvisoWidget* GetProvisoWidgetInstance = CreateWidget<UZeroGetProvisoWidget>(GetWorld(), GetProvisoWidgetClass);
	if (GetProvisoWidgetInstance)
	{
		GetProvisoWidgetInstance->ShowWidget();
		ZE_LOG(LogZeroSector, Display, TEXT("GetProvisoWidget UI 표시됨"));
	}
}

void AZeroCharacterPlayer::OperationBoardInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("OperationBoard")))
	{
		OperationUITest();
	}
}

void AZeroCharacterPlayer::SetInputByDaySequence(EDaySequence DaySequence)
{
	ChangeInputMap[DaySequence].ChangeInput.ExecuteIfBound();
}

void AZeroCharacterPlayer::SetInputAfternoonMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputConfig->IMC_Day, 0);
		ZE_LOG(LogZeroSector, Display, TEXT("Afternoon InputMode"));
	}
}

void AZeroCharacterPlayer::SetInputNightMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputConfig->IMC_Night, 0);
		ZE_LOG(LogZeroSector, Display, TEXT("Night InputMode"));
	}
}

void AZeroCharacterPlayer::InteractBeam()
{
	FVector EyeVectorStart;
	FRotator EyeRotatorStart;
	GetController()->GetPlayerViewPoint(EyeVectorStart, EyeRotatorStart);

	FVector EyeVectorEnd = EyeVectorStart + EyeRotatorStart.Vector() * DetectDistance;
	FHitResult HitResult;
	FCollisionQueryParams Param(NAME_None, false, this);
	FColor Color(FColor::Red);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeVectorStart, EyeVectorEnd, ECC_GameTraceChannel1, Param);
	if (bHit)
	{
		Color = FColor::Green;
		AActor* HitActor = HitResult.GetActor();

		for (UActorComponent* ActorComp : HitResult.GetActor()->GetComponentsByInterface(UZeroDialogueInterface::StaticClass()))
		{
			DialogueInterface = Cast<IZeroDialogueInterface>(ActorComp);
			//DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
			return;
		}

		/* 
			AZeroGimmick 을 베이스 클래스로 상호작용 할 수 있는 물품들을 클래스 작성
			이들을 Tag로 분류하고 다형성을 이용해서 포인터에 저장
		*/
		if (HitActor->ActorHasTag(TEXT("Proviso")))
		{
			InteractedGimmick = Cast<AZeroProvisoActor>(HitActor);

			
			if (!ProvisoWidgetInstance)
			{
				if (ProvisoWidgetClass)
				{
					ProvisoWidgetInstance = CreateWidget<UZeroProvisoWidget>(GetWorld(), ProvisoWidgetClass);
				}
			}

			if (ProvisoWidgetInstance)
			{
				ProvisoWidgetInstance->ShowWidget();
			}
			
		}
		else if (HitActor->ActorHasTag(TEXT("OperationBoard")))
		{
			InteractedGimmick = Cast<AZeroOperationBoard>(HitActor);
		}
	}
	else
	{
		DialogueInterface = nullptr;
		InteractedGimmick = nullptr;
		if (ProvisoWidgetInstance) 
		{
			ProvisoWidgetInstance->RemoveFromParent();
		}
	}

	//DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
}

void AZeroCharacterPlayer::SetDefaultMovement()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	CameraComp->SetRelativeLocation(CameraData->CommonCameraVector);
	CameraComp->SetRelativeRotation(CameraData->CommonCameraRotator);
}

void AZeroCharacterPlayer::SetDialogueMovement()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	CameraComp->SetRelativeLocation(CameraData->DialogueCameraVector);
	CameraComp->SetRelativeRotation(CameraData->DialogueCameraRotator);
}

void AZeroCharacterPlayer::SetRifle()
{
	CurrentWeaponType = EWeaponType::ERifle;
	RifleMeshComp->SetSkeletalMesh(RifleMesh);
	PistolMeshComp->SetSkeletalMesh(nullptr);
}

void AZeroCharacterPlayer::SetPistol()
{
	CurrentWeaponType = EWeaponType::EPistol;
	PistolMeshComp->SetSkeletalMesh(PistolMesh);
	RifleMeshComp->SetSkeletalMesh(nullptr);
}

void AZeroCharacterPlayer::SetShotgun()
{
	CurrentWeaponType = EWeaponType::EShotgun;
	RifleMeshComp->SetSkeletalMesh(ShotgunMesh);
	PistolMeshComp->SetSkeletalMesh(nullptr);
}

void AZeroCharacterPlayer::OperationUITest()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetPlayerController());
	if (PC)
	{
		PC->InputModeUIOnly();
	}

	OperationWidgetPtr = CreateWidget<UZeroOperationWidget>(GetPlayerController(), OperationWidgetClass);
	if (OperationWidgetPtr)
	{
		OperationWidgetPtr->AddToViewport();
		FOnClickNextButton OnClickNextButton;
		OnClickNextButton.BindLambda([&]()
			{
				ClickNextButton();
			});
		OperationWidgetPtr->SetDelegateClickNextButton(OnClickNextButton);
	}
}

void AZeroCharacterPlayer::ClickNextButton()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetPlayerController());
	if (PC)
	{
		PC->InputModeGameOnly();
	}

	OperationWidgetPtr->RemoveFromParent();

	switch (OperationWidgetPtr->GetWeaponType())
	{
	case EWeaponType::ERifle:
		Weapons.Add(EWeaponType::ERifle, GetWorld()->SpawnActor<AZeroWeaponRifle>(AZeroWeaponRifle::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		ChoicedWeapon = EWeaponType::ERifle;
		break;
	case EWeaponType::EShotgun:
		Weapons.Add(EWeaponType::EShotgun, GetWorld()->SpawnActor<AZeroWeaponShotgun>(AZeroWeaponShotgun::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		ChoicedWeapon = EWeaponType::EShotgun;
		break;
	default:
		ZE_LOG(LogZeroSector, Error, TEXT("무기 안들어옴"));
		break;
	}

	for (const auto& Weapon : Weapons)
	{
		Weapon.Value->SetOwner(this);
	}

	FadeInAndOutWidgetPtr = CreateWidget<UZeroFadeInAndOutWidget>(GetPlayerController(), FadeInAndOutWidgetClass);
	FadeInAndOutWidgetPtr->AddToViewport();
	FadeInAndOutWidgetPtr->FadeInPlay();

	SetInputByDaySequence(EDaySequence::ENight);
	SetPistol();
}
