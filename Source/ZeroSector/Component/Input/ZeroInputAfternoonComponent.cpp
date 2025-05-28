// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Input/ZeroInputAfternoonComponent.h"
#include "Component/ZeroUIComponent.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Data/ZeroSingleton.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "Gimmick/ZeroOperationBoard.h"
#include "Gimmick/ZeroEnforceBoard.h"
#include "Game/ZeroSoundManager.h"
#include "Game/ZeroGameInstance.h"
#include "Component/ZeroPlayerStatComponent.h"
#include "Interface/ZeroDialogueInterface.h"
#include "Interface/ZeroAfternoonInputInterface.h"
#include "Interface/ZeroOutlineInterface.h"
#include "UI/ZeroPauseMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroSector.h"

AActor* UZeroInputAfternoonComponent::CurrentDialogueNPC;

UZeroInputAfternoonComponent::UZeroInputAfternoonComponent()
{
	DetectDistance = 800.f;
}

void UZeroInputAfternoonComponent::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	FRotator Rotation = Player->Controller->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Player->AddMovementInput(ForwardVector, InputValue.X);
	Player->AddMovementInput(RightVector, InputValue.Y);
}

void UZeroInputAfternoonComponent::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	Player->AddControllerPitchInput(-InputValue.Y);
	Player->AddControllerYawInput(InputValue.X);
}

void UZeroInputAfternoonComponent::Run()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = 500.f;

	SetFootstepInterval(0.3f);
}

void UZeroInputAfternoonComponent::Walk()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = 300.f;

	SetFootstepInterval(0.5f);
}

void UZeroInputAfternoonComponent::InteractBeam()
{
	FVector EyeVectorStart;
	FRotator EyeRotatorStart;
	Player->GetController()->GetPlayerViewPoint(EyeVectorStart, EyeRotatorStart);

	FVector EyeVectorEnd = EyeVectorStart + EyeRotatorStart.Vector() * DetectDistance;
	FHitResult HitResult;
	FCollisionQueryParams Param(NAME_None, false, GetOwner());
	//DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, FColor::Red, false);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeVectorStart, EyeVectorEnd, ECC_GameTraceChannel1, Param);
	InteractProcess(HitResult, bHit);
}

void UZeroInputAfternoonComponent::InteractProcess(const FHitResult& InHitResult, bool bIsHit)
{
	AActor* HitActor = InHitResult.GetActor();
	if (PrevGimmick != HitActor)
	{
		AZeroProvisoActor* ProvisoActor = Cast<AZeroProvisoActor>(PrevGimmick);
		if (ProvisoActor)
		{
			if (ProvisoActor->ProvisoType != EZeroProvisoType::Normal)
			{
				IZeroOutlineInterface* OutlineInterface = Cast<IZeroOutlineInterface>(PrevGimmick);
				if (OutlineInterface)
				{
					OutlineInterface->SetUnOverlayMaterial();
				}
			}
		}
	}
	PrevGimmick = HitActor;

	if (bIsHit)
	{
		IZeroOutlineInterface* OutlineInterface = Cast<IZeroOutlineInterface>(HitActor);
		if (OutlineInterface) OutlineInterface->SetOverlayMaterial();

		for (UActorComponent* ActorComp : InHitResult.GetActor()->GetComponentsByInterface(UZeroDialogueInterface::StaticClass()))
		{
			DialogueInterface = Cast<IZeroDialogueInterface>(ActorComp);
			CurrentDialogueNPC = PrevGimmick;
			return;
		}

		if (HitActor->ActorHasTag(TEXT("Proviso")))
		{
			UZeroUIComponent* UIComp = Player->FindComponentByClass<UZeroUIComponent>();
			if (UIComp)
			{
				UIComp->CurrentGimmick = HitActor;
			}

			InteractBeamReachedProviso(HitActor);
		}
		else if (HitActor->ActorHasTag(TEXT("OperationBoard")))
		{
			InteractedGimmick = Cast<AZeroOperationBoard>(HitActor);
		}
		else if (HitActor->ActorHasTag(TEXT("EnforceBoard")))
		{
			InteractedGimmick = Cast<AZeroEnforceBoard>(HitActor);
		}
	}
	else
	{
		DialogueInterface = nullptr;
		InteractedGimmick = nullptr;
		CurrentDialogueNPC = nullptr;
		IZeroAfternoonInputInterface* Interface = Cast<IZeroAfternoonInputInterface>(Player);
		if (Interface) Interface->CloseInteractUI();
	}
}

void UZeroInputAfternoonComponent::InteractBeamReachedProviso(AActor* InHitActor)
{
	InteractedGimmick = Cast<AZeroProvisoActor>(InHitActor);
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("Proviso")))
	{
		IZeroAfternoonInputInterface* Interface = Cast<IZeroAfternoonInputInterface>(Player);
		if (Interface)
		{
			Interface->DisplayInteractUI();
		}
	}
}

void UZeroInputAfternoonComponent::DialogueInteract()
{
	if (DialogueInterface && StatComp->GetCanInteract())
	{
		DialogueInterface->StartDialogue();
		FOnFinishedDialogue OnFinishedDialogue;
		OnFinishedDialogue.BindLambda([&]()
			{
				SetDefaultMovement();
			});
		DialogueInterface->SetupFinishedDialogueDelegate(OnFinishedDialogue);

		SetDialogueMovement();

		StatComp->UseActivePoint(-10.f);
	}
}

void UZeroInputAfternoonComponent::OperationBoardInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("OperationBoard")))
	{
		OnOperationInteract.ExecuteIfBound();

		UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
		if (GI && GI->GetSoundManager() && GI->GetSoundManager()->OperationSFX)
		{
			UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->OperationSFX);
		}
	}
}

void UZeroInputAfternoonComponent::ProvisoInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("Proviso")) && StatComp->GetCanInteract())
	{
		OnProvisoInteract.ExecuteIfBound();

		UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
		if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ProvisoInteractSFX)
		{
			UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->ProvisoInteractSFX);
		}

		StatComp->UseActivePoint(-10.f);
	}

	
}

void UZeroInputAfternoonComponent::EnforceBoardInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("EnforceBoard")))
	{
		OnEnforceInteract.ExecuteIfBound();

		UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
		if (GI && GI->GetSoundManager() && GI->GetSoundManager()->EnforceSFX)
		{
			UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->EnforceSFX);
		}
	}
}

void UZeroInputAfternoonComponent::ToggleNote()
{
	OnNoteDisplay.ExecuteIfBound();
}

void UZeroInputAfternoonComponent::PauseMenu()
{
	OnPauseMenu.ExecuteIfBound();
}

void UZeroInputAfternoonComponent::ExcludeResearcher()
{
	OnExcludeResearcher.ExecuteIfBound();
}

void UZeroInputAfternoonComponent::SelectResearcher()
{
	OnSelectResearcher.ExecuteIfBound();
}

void UZeroInputAfternoonComponent::BeginPlay()
{
	Super::BeginPlay();
	
	check(Player);

	GetWorld()->GetTimerManager().SetTimer(FootstepTimerHandle, this, &UZeroInputAfternoonComponent::TryPlayFootstepSound, 0.5f, true);
	StatComp = Player->GetComponentByClass<UZeroPlayerStatComponent>(); // 바인딩이 된 포인터 가져오기
}

void UZeroInputAfternoonComponent::SetDefaultMovement()
{
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UZeroInputAfternoonComponent::SetDialogueMovement()
{
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void UZeroInputAfternoonComponent::TryPlayFootstepSound()
{
	if (!Player) return;

	if (Player->GetVelocity().Size() > 10.f)
	{
		UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
		if (GI && GI->GetSoundManager() && GI->GetSoundManager()->FootstepSFX)
		{
			UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->FootstepSFX);
		}
	}
}

void UZeroInputAfternoonComponent::SetFootstepInterval(float NewInterval)
{
	if (FMath::IsNearlyEqual(CurrentFootstepInterval, NewInterval, 0.01f)) return;

	CurrentFootstepInterval = NewInterval;
	GetWorld()->GetTimerManager().ClearTimer(FootstepTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(FootstepTimerHandle, this, &UZeroInputAfternoonComponent::TryPlayFootstepSound, NewInterval, true);
}
