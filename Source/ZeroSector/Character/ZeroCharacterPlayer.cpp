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
#include "ZeroSector.h"

AZeroCharacterPlayer::AZeroCharacterPlayer()
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

void AZeroCharacterPlayer::InteractBeam()
{
	FVector EyeVectorStart;
	FRotator EyeRotatorStart;
	GetController()->GetPlayerViewPoint(EyeVectorStart, EyeRotatorStart);

	const float SightDistance = 800.f;
	FVector EyeVectorEnd = EyeVectorStart + EyeRotatorStart.Vector() * SightDistance;

	FHitResult HitResult;
	FCollisionQueryParams Param(NAME_None, false, this);
	FColor Color(FColor::Red);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeVectorStart, EyeVectorEnd, ECC_GameTraceChannel1, Param);
	if (bHit)
	{
		Color = FColor::Green;
		for (UActorComponent* ActorComp : HitResult.GetActor()->GetComponentsByInterface(UZeroDialogueInterface::StaticClass()))
		{
			DialogueInterface = Cast<IZeroDialogueInterface>(ActorComp);
			DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
			return;
		}

		/* 여기에서 단서 관련 이벤트 처리 */
		/* 예시) 헤더파일에 Proviso 액터 포인터 선언 
				 단서는 Actor 클래스를 상속받아서 구현하면 될듯
		*/
		Proviso = HitResult.GetActor();
	}
	DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
}
