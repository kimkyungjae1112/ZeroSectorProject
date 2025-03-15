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
#include "Components/SphereComponent.h"
#include "Game/ProvisoActor.h"
#include "Interface/ZeroDialogueInterface.h"
#include "UI/ZeroOperationWidget.h"
#include "UI/ZeroFadeInAndOutWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Weapon/ZeroWeaponRifle.h"
#include "Weapon/ZeroWeaponShotgun.h"
#include "Gimmick/ZeroOperationBoard.h"
#include "ZeroSector.h"

AZeroCharacterPlayer::AZeroCharacterPlayer() : DetectDistance(800.f)
{
	PrimaryActorTick.bCanEverTick = true;

	// 임시로 메쉬 지정
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (BodyMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(BodyMeshRef.Object);
	}
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

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(GetMesh());
	CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetRelativeLocation(CameraData->CommonCameraVector);
	CameraComp->SetRelativeRotation(CameraData->CommonCameraRotator);
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
	EnhancedInputComponent->BindAction(InputConfig->IA_OperationTest, ETriggerEvent::Started, this, &AZeroCharacterPlayer::OperationUITest);
}

void AZeroCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputConfig->IMC_Default, 0);
	}
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

void AZeroCharacterPlayer::OperationBoardInteract()
{
	if (OperationBoard)
	{
		OperationUITest();
	}
}

void AZeroCharacterPlayer::Fire()
{
	Weapon->Fire();
	// 무기 기능 구현은 애니메이션 및 애셋 생기면 구현하기
}

void AZeroCharacterPlayer::Aiming()
{
	Weapon->Aiming();
	// 무기 기능 구현은 애니메이션 및 애셋 생기면 구현하기
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



void AZeroCharacterPlayer::ProvisoInteract()
{

	if (DetectedProviso)
	{
		UE_LOG(LogTemp, Log, TEXT("단서 획득: %s"), *DetectedProviso->GetName());
		DetectedProviso->Destroy();
		DetectedProviso = nullptr;
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

		if (!HitActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("라인 트레이스가 Actor를 감지하지 못함."));
			return;
		}

		for (UActorComponent* ActorComp : HitResult.GetActor()->GetComponentsByInterface(UZeroDialogueInterface::StaticClass()))
		{
			DialogueInterface = Cast<IZeroDialogueInterface>(ActorComp);
			DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
			return;
		}

		DetectedProviso = Cast<AProvisoActor>(HitActor);
		if (DetectedProviso)
		{
			if (!DetectedProviso->IsValidLowLevelFast())
			{
				// UE_LOG(LogTemp, Error, TEXT("AProvisoActor가 유효하지 않음!"));
				return;
			}

			// ShowInteractionUI(true);
			// UE_LOG(LogTemp, Log, TEXT("단서 감지됨: %s"), *Proviso->GetName());
			DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
			return;
		}
		
		DialogueInterface = nullptr;

		OperationBoard = Cast<AZeroOperationBoard>(HitResult.GetActor());
	}
	DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
	// ShowInteractionUI(false);
	DetectedProviso = nullptr;
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
		OperationWidgetPtr->AddToViewport(1);
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
	case EWeaponType::EZeroRifle:
		Weapon = GetWorld()->SpawnActor<AZeroWeaponRifle>(AZeroWeaponRifle::StaticClass());
		if (Weapon) ZE_LOG(LogZeroSector, Display, TEXT("Weapon Name : %s"), *Weapon->GetActorNameOrLabel());
		break;
	case EWeaponType::EZeroShotgun:
		Weapon = GetWorld()->SpawnActor<AZeroWeaponShotgun>(AZeroWeaponShotgun::StaticClass());
		if (Weapon) ZE_LOG(LogZeroSector, Display, TEXT("Weapon Name : %s"), *Weapon->GetActorNameOrLabel());
		break;
	default:
		ZE_LOG(LogZeroSector, Error, TEXT("무기 안들어옴"));
		break;
	}

	FadeInAndOutWidgetPtr = CreateWidget<UZeroFadeInAndOutWidget>(GetPlayerController(), FadeInAndOutWidgetClass);
	FadeInAndOutWidgetPtr->AddToViewport();
	FadeInAndOutWidgetPtr->FadeInPlay();
}
