// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterPlayer.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Data/ZeroInputConfig.h"
#include "Data/ZeroPlayerCameraData.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Interface/ZeroDialogueInterface.h"

AZeroCharacterPlayer::AZeroCharacterPlayer()
{
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

	DialogueSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Dialogue Sphere Component"));
	DialogueSphereComp->SetupAttachment(RootComponent);
	DialogueSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AZeroCharacterPlayer::BeginOverlapForDialogue);
	DialogueSphereComp->SetSphereRadius(200.f);
}

void AZeroCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(InputConfig->IA_Move, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(InputConfig->IA_Look, ETriggerEvent::Triggered, this, &AZeroCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &AZeroCharacterPlayer::DialogueInteract);
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

void AZeroCharacterPlayer::BeginOverlapForDialogue(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		for (UActorComponent* ActorComp : OtherActor->GetComponentsByInterface(UZeroDialogueInterface::StaticClass()))
		{
			// 개선 방향
				// 1. 모든 ActorComp의 소유 액터들의 거리를 계산해서 가장 짧은 액터의 인터페이스를 가져올 수 있도록 수정
				// 2. 각도를 계산해서 아주 작은 각도 내에 들어와 있는 액터의 인터페이스를 가져올 수 있도록 수정
				// 3. SingleLineTrace를 사용해서 특정 액터의 인터페이스를 가져올 수 있도록 수정 (Tick 사용)
			DialogueInterface = Cast<IZeroDialogueInterface>(ActorComp);
		}
	}
}
