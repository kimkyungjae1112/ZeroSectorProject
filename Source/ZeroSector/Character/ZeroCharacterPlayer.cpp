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
				NoteWidgetPtr->SetNoteInfo(ProvisoData);
			}
		}

		ProvisoNum = 1;
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

		GetZeroPlayerController()->SetInputMode(FInputModeGameOnly());
		GetZeroPlayerController()->bShowMouseCursor = false;
	}
	else
	{
		NoteWidgetPtr->AddToViewport();
		bIsNoteToggle = true;

		// 게임 + UI 모드로 전환하되, 키보드 포커스를 Note 위젯에 주지 않음
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(nullptr); // 또는 특정 포커스 위젯
		GetZeroPlayerController()->SetInputMode(InputMode);
		GetZeroPlayerController()->bShowMouseCursor = true;
	}
}
