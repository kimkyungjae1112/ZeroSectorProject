// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterPlayer.h"
#include "ZeroHeader/ZeroInputHeader.h"
#include "ZeroHeader/ZeroUIHeader.h"
#include "ZeroHeader/ZeroWeaponHeader.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/ZeroPlayerCameraData.h"
#include "Interface/ZeroDialogueInterface.h"
#include "Player/ZeroPlayerController.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "Gimmick/ZeroOperationBoard.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Data/ZeroSingleton.h"
#include "Component/Input/ZeroInputBaseComponent.h"
#include "ZeroSector.h"

AZeroCharacterPlayer::AZeroCharacterPlayer() : DetectDistance(800.f)
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UZeroInputConfig> InputConfigRef(TEXT("/Script/ZeroSector.ZeroInputData'/Game/Data/Input/DataAsset/DA_InputData.DA_InputData'"));
	if (InputConfigRef.Object)
	{
		InputConfig = InputConfigRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UZeroPlayerCameraData> CameraDataRef(TEXT("/Script/ZeroSector.ZeroPlayerCameraData'/Game/Data/Camera/DA_CameraData.DA_CameraData'"));
	if (CameraDataRef.Object)
	{
		CameraData = CameraDataRef.Object;
	}
	
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComp->SetupAttachment(GetMesh(), TEXT("headSocket"));
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);

	InputComp = CreateDefaultSubobject<UZeroInputBaseComponent>(TEXT("Input Config Component"));

	ChangeInputMap.Add(EDaySequence::EAfternoon, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputAfternoonMode)));
	ChangeInputMap.Add(EDaySequence::ENight, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputNightMode)));

	CurrentWeaponType = EWeaponType::ENone;
	TeamId = FGenericTeamId(0);
	ClassName = TEXT("Player");
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
	EnhancedInputComponent->BindAction(InputConfig->IA_NightToAfternoon, ETriggerEvent::Started, this, &AZeroCharacterPlayer::NightToAfternoon);
	EnhancedInputComponent->BindAction(InputConfig->IA_ToggleNote, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ToggleNote);
	EnhancedInputComponent->BindAction(InputConfig->IA_Run, ETriggerEvent::Started, this, &AZeroCharacterPlayer::Run);
	EnhancedInputComponent->BindAction(InputConfig->IA_Run, ETriggerEvent::Completed, this, &AZeroCharacterPlayer::Walk);
	EnhancedInputComponent->BindAction(InputConfig->IA_Reloading, ETriggerEvent::Started, this, &AZeroCharacterPlayer::Reloading);
}

FGenericTeamId AZeroCharacterPlayer::GetGenericTeamId() const
{
	return TeamId;
}

void AZeroCharacterPlayer::SetHUDWidget(UZeroHUDWidget* InHUDWidget)
{
	HUDWidgetPtr = InHUDWidget;
}

void AZeroCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	Walk();
	SetInputAfternoonMode();
	NoteWidgetPtr = CreateWidget<UZeroNoteWidget>(GetWorld(), NoteWidgetClass);
}

APlayerController* AZeroCharacterPlayer::GetPlayerController() const
{
	return CastChecked<APlayerController>(GetController());
}

AZeroPlayerController* AZeroCharacterPlayer::GetZeroPlayerController() const
{
	return CastChecked<AZeroPlayerController>(GetController());
}

void AZeroCharacterPlayer::Move(const FInputActionValue& Value)
{
	InputComp->Move(Value);
}

void AZeroCharacterPlayer::Look(const FInputActionValue& Value)
{
	InputComp->Look(Value);
}

void AZeroCharacterPlayer::Run()
{
	InputComp->Run();
}

void AZeroCharacterPlayer::Walk()
{
	InputComp->Walk();
}

void AZeroCharacterPlayer::Fire()
{
	CurrentWeapon->Fire();
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
		if (CurrentWeaponType == EWeaponType::EPistol)
		{
			SetRifle();
			CurrentWeapon->GunAmmoTextDisplay();
		}
		else
		{
			SetPistol();
			CurrentWeapon->GunAmmoTextDisplay();
		}
	}
	else if (ChoicedWeapon == EWeaponType::EShotgun)
	{
		if (CurrentWeaponType == EWeaponType::EPistol)
		{
			SetShotgun();
			CurrentWeapon->GunAmmoTextDisplay();
		}
		else
		{
			SetPistol();
			CurrentWeapon->GunAmmoTextDisplay();
		}
	}
}

void AZeroCharacterPlayer::Reloading()
{
	CurrentWeapon->ReloadingCurrentAmmo();
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
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("Proviso")))
	{
		UZeroGetProvisoWidget* GetProvisoWidgetInstance = CreateWidget<UZeroGetProvisoWidget>(GetWorld(), GetProvisoWidgetClass);
		if (GetProvisoWidgetInstance)
		{
			GetProvisoWidgetInstance->ShowWidget();
		}

		FZeroProvisoDataTable ProvisoData = UZeroSingleton::Get().GetProvisoData(ProvisoNum);

		if (!ProvisoData.ProvisoName.IsNone())
		{
			UZeroSingleton::Get().AddCollectedProviso(ProvisoData);

			GetProvisoWidgetInstance->SetProvisoInfo(ProvisoData.ProvisoName.ToString(), ProvisoData.Description);

			if (NoteWidgetPtr)
			{
				NoteWidgetPtr->SetNoteInfo(ProvisoData.ProvisoName.ToString(), ProvisoData.Description);
			}
		}

		ProvisoNum = 1;
	}
	
}

void AZeroCharacterPlayer::OperationBoardInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("OperationBoard")))
	{
		OperationWidgetDisplay();
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

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeVectorStart, EyeVectorEnd, ECC_GameTraceChannel1, Param);
	InteractProcess(HitResult, bHit);
}

void AZeroCharacterPlayer::InteractProcess(const FHitResult& InHitResult, bool bIsHit)
{
	FColor Color(FColor::Red);
	if (bIsHit)
	{
		Color = FColor::Green;
		AActor* HitActor = InHitResult.GetActor();

		for (UActorComponent* ActorComp : InHitResult.GetActor()->GetComponentsByInterface(UZeroDialogueInterface::StaticClass()))
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
			InteractBeamReachedProviso(HitActor);
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
		if (ProvisoWidgetPtr)
		{
			ProvisoWidgetPtr->RemoveFromParent();
		}
	}
	//DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
}

void AZeroCharacterPlayer::InteractBeamReachedProviso(AActor* InHitActor)
{
	InteractedGimmick = Cast<AZeroProvisoActor>(InHitActor);
	if (!ProvisoWidgetPtr)
	{
		if (ProvisoWidgetClass)
		{
			ProvisoWidgetPtr = CreateWidget<UZeroProvisoWidget>(GetWorld(), ProvisoWidgetClass);
		}
	}
	if (ProvisoWidgetPtr)
	{
		ProvisoWidgetPtr->ShowWidget();
	}
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

void AZeroCharacterPlayer::SetNoWeapon()
{
	CurrentWeaponType = EWeaponType::ENone;
	CurrentWeapon = nullptr;

	for (const auto& Weapon : Weapons)
	{
		Weapon.Value->Destroy();
	}
}

void AZeroCharacterPlayer::SetRifle()
{	
	CurrentWeaponType = EWeaponType::ERifle;
	CurrentWeapon = Weapons[EWeaponType::ERifle];
	
	SetupTransformWeapon(TEXT("hand_rRifle"));
	ChangeWeaponMesh();
}

void AZeroCharacterPlayer::SetPistol()
{
	CurrentWeaponType = EWeaponType::EPistol;
	CurrentWeapon = Weapons[EWeaponType::EPistol];

	SetupTransformWeapon(TEXT("hand_rPistol"));
	ChangeWeaponMesh();
}

void AZeroCharacterPlayer::SetShotgun()
{
	CurrentWeaponType = EWeaponType::EShotgun;
	CurrentWeapon = Weapons[EWeaponType::EShotgun];
	
	SetupTransformWeapon(TEXT("hand_rShotgun"));
	ChangeWeaponMesh();
}

void AZeroCharacterPlayer::SetupTransformWeapon(const FName& SocketName)
{
	FVector Loc = GetMesh()->GetSocketLocation(SocketName);
	FRotator Rot = GetMesh()->GetSocketRotation(SocketName);
	CurrentWeapon->SetActorLocation(Loc);
	CurrentWeapon->SetActorRotation(Rot);
}

void AZeroCharacterPlayer::ChangeWeaponMesh()
{
	FVector Loc = GetMesh()->GetSocketLocation(TEXT("WeaponInventory"));
	FRotator Rot = GetMesh()->GetSocketRotation(TEXT("WeaponInventory"));

	for (const auto& Weapon : Weapons)
	{
		if (Weapon.Value != Weapons[CurrentWeaponType])
		{
			Weapon.Value->SetActorLocation(Loc);
			Weapon.Value->SetActorRotation(Rot);
		}
	}
}

void AZeroCharacterPlayer::OperationWidgetDisplay()
{
	GetZeroPlayerController()->InputModeUIOnly();
	
	OperationWidgetPtr = CreateWidget<UZeroOperationWidget>(GetPlayerController(), OperationWidgetClass);
	if (OperationWidgetPtr)
	{
		OperationWidgetPtr->AddToViewport();
		FOnClickNextButton OnClickNextButton;
		OnClickNextButton.BindLambda([&]()
			{
				OperationNextButtonClick();
			});
		OperationWidgetPtr->SetDelegateClickNextButton(OnClickNextButton);
	}
}

void AZeroCharacterPlayer::OperationNextButtonClick()
{
	GetZeroPlayerController()->InputModeGameOnly();

	OperationWidgetPtr->RemoveFromParent();

	switch (OperationWidgetPtr->GetWeaponType())
	{
	case EWeaponType::ERifle:
		Weapons.Add(EWeaponType::ERifle, GetWorld()->SpawnActor<AZeroWeaponRifle>(AZeroWeaponRifle::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		Weapons[EWeaponType::ERifle]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rRifle"));
		Weapons[EWeaponType::EPistol]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rPistol"));
		ChoicedWeapon = EWeaponType::ERifle;
		break;
	case EWeaponType::EShotgun:
		Weapons.Add(EWeaponType::EShotgun, GetWorld()->SpawnActor<AZeroWeaponShotgun>(AZeroWeaponShotgun::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		Weapons[EWeaponType::EShotgun]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rShotgun"));
		Weapons[EWeaponType::EPistol]->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rPistol"));
		ChoicedWeapon = EWeaponType::EShotgun;
		break;
	default:
		ZE_LOG(LogZeroSector, Error, TEXT("무기 안들어옴"));
		break;
	}

	for (const auto& Weapon : Weapons)
	{
		Weapon.Value->SetOwner(this);
		Weapon.Value->OnSetMaxAmmo.BindUObject(HUDWidgetPtr, &UZeroHUDWidget::UpdateMaxAmmo);
		Weapon.Value->OnChangedAmmo.BindUObject(HUDWidgetPtr, &UZeroHUDWidget::UpdateCurrentAmmo);
	}

	FadeInAndOutDisplay();

	SetInputByDaySequence(EDaySequence::ENight);
	SetPistol();
	CurrentWeapon->GunAmmoTextDisplay();
}

void AZeroCharacterPlayer::FadeInAndOutDisplay()
{
	FadeInAndOutWidgetPtr = CreateWidget<UZeroFadeInAndOutWidget>(GetPlayerController(), FadeInAndOutWidgetClass);
	FadeInAndOutWidgetPtr->AddToViewport();
	FadeInAndOutWidgetPtr->FadeInPlay();
	FadeInAndOutWidgetPtr = nullptr;
}

void AZeroCharacterPlayer::NightToAfternoon()
{
	FadeInAndOutDisplay();

	SetInputByDaySequence(EDaySequence::EAfternoon);
	SetNoWeapon();
}

void AZeroCharacterPlayer::ToggleNote()
{

	if (bIsNoteToggle)
	{
		NoteWidgetPtr->RemoveFromParent();
		bIsNoteToggle = false;
	}
	else
	{
		NoteWidgetPtr->AddToViewport();
		bIsNoteToggle = true;
	}
}