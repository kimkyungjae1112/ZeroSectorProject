// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterPlayer.h"
#include "ZeroHeader/ZeroInputHeader.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Component/Input/ZeroInputBaseComponent.h"
#include "Component/Input/ZeroInputAfternoonComponent.h"
#include "Component/Input/ZeroInputNightComponent.h"
#include "Component/ZeroUIComponent.h"
#include "Player/ZeroPlayerController.h"
#include "ZeroSector.h"

AZeroCharacterPlayer::AZeroCharacterPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UZeroInputConfig> InputConfigRef(TEXT("/Script/ZeroSector.ZeroInputData'/Game/Data/Input/DataAsset/DA_InputData.DA_InputData'"));
	if (InputConfigRef.Object)
	{
		InputConfig = InputConfigRef.Object;
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
	UIComp = CreateDefaultSubobject<UZeroUIComponent>(TEXT("UI Component"));

	ChangeInputMap.Add(EDaySequence::EAfternoon, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputAfternoonMode)));
	ChangeInputMap.Add(EDaySequence::ENight, FChangeInputWrapper(FChangeInput::CreateUObject(this, &AZeroCharacterPlayer::SetInputNightMode)));

	CurrentWeaponType = EWeaponType::ENone;
	TeamId = FGenericTeamId(0);
	ClassName = TEXT("Player");
}

void AZeroCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (InputComp && InputComp->IsA(UZeroInputAfternoonComponent::StaticClass()))
	{
		//ZE_LOG(LogZeroSector, Display, TEXT("Tick 실행?"));
		InputComp->InteractBeam();
	}
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
	EnhancedInputComponent->BindAction(InputConfig->IA_ChangeWeapon, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ChangeWeapon);
	EnhancedInputComponent->BindAction(InputConfig->IA_NightToAfternoon, ETriggerEvent::Started, this, &AZeroCharacterPlayer::NightToAfternoon);
	EnhancedInputComponent->BindAction(InputConfig->IA_ToggleNote, ETriggerEvent::Started, this, &AZeroCharacterPlayer::ToggleNoteDisplay);
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

void AZeroCharacterPlayer::DisplayInteractUI()
{
	UIComp->InteractUIDisplay();
}

void AZeroCharacterPlayer::CloseInteractUI()
{
	UIComp->InteractUIClose();
}

#if WITH_EDITOR
void AZeroCharacterPlayer::NightToAfternoon()
{
	UIComp->FadeInAndOutDisplay();
	SetInputByDaySequence(EDaySequence::EAfternoon);
}
#endif

AZeroPlayerController* AZeroCharacterPlayer::GetOwnerController()
{
	return GetZeroPlayerController();
}

void AZeroCharacterPlayer::ChangeInputMode()
{
	if (InputComp && InputComp->IsA(UZeroInputAfternoonComponent::StaticClass()))
	{
		SetInputByDaySequence(EDaySequence::ENight);
		UIComp->OnClickOperationNextButton.BindUObject(InputComp, &UZeroInputBaseComponent::SetupWeapon);
		CurrentWeaponType = InputComp->GetWeaponType();
	}
	else
	{
		SetInputByDaySequence(EDaySequence::EAfternoon);
	}
}

void AZeroCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InputComp = NewObject<UZeroInputNightComponent>(this, UZeroInputNightComponent::StaticClass());
	InputComp->RegisterComponent();
}

void AZeroCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();


	Walk();
	SetInputAfternoonMode();
	InputComp->OnOperationInteract.BindUObject(UIComp, &UZeroUIComponent::OperationInteract);
	InputComp->OnProvisoInteract.BindUObject(UIComp, &UZeroUIComponent::ProvisoInteract);
	InputComp->OnNoteDisplay.BindUObject(UIComp, &UZeroUIComponent::ToggleNoteDisplay);
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
	if (InputComp)
	{
		InputComp->Move(Value);
	}
}

void AZeroCharacterPlayer::Look(const FInputActionValue& Value)
{
	if (InputComp)
	{
		InputComp->Look(Value);
	}
}

void AZeroCharacterPlayer::Run()
{
	if (InputComp)
	{
		InputComp->Run();
	}
}

void AZeroCharacterPlayer::Walk()
{
	if (InputComp)
	{
		InputComp->Walk();
	}
}

void AZeroCharacterPlayer::Fire()
{
	if (InputComp)
	{
		InputComp->Fire();
	}
}

void AZeroCharacterPlayer::ChangeWeapon()
{
	if (InputComp)
	{
		InputComp->ChangeWeapon();
		CurrentWeaponType = InputComp->GetWeaponType();
	}
}

void AZeroCharacterPlayer::Reloading()
{
	if (InputComp)
	{
		InputComp->Reloading();
	}
}

void AZeroCharacterPlayer::DialogueInteract()
{
	if (InputComp)
	{
		InputComp->DialogueInteract();
	}
}

void AZeroCharacterPlayer::ProvisoInteract()
{	
	if (InputComp)
	{
		InputComp->ProvisoInteract();
	}
}

void AZeroCharacterPlayer::OperationBoardInteract()
{
	if (InputComp)
	{
		InputComp->OperationBoardInteract();
	}
}

void AZeroCharacterPlayer::ToggleNoteDisplay()
{
	if (InputComp)
	{
		InputComp->ToggleNote();
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
		if (InputComp)
		{
			InputComp->DestroyComponent();
			InputComp = nullptr;
		}

		InputComp = NewObject<UZeroInputAfternoonComponent>(this, UZeroInputAfternoonComponent::StaticClass());
		InputComp->RegisterComponent();

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputConfig->IMC_Day, 0);
	}
}

void AZeroCharacterPlayer::SetInputNightMode()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerController()->GetLocalPlayer()))
	{
		if (InputComp)
		{
			InputComp->DestroyComponent();
			InputComp = nullptr;
		}

		InputComp = NewObject<UZeroInputNightComponent>(this, UZeroInputNightComponent::StaticClass());
		InputComp->RegisterComponent();

		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputConfig->IMC_Night, 0);
	}
}
