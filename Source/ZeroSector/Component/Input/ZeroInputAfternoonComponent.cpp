// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Input/ZeroInputAfternoonComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "Gimmick/ZeroOperationBoard.h"
#include "Interface/ZeroDialogueInterface.h"
#include "Interface/ZeroAfternoonInputInterface.h"
#include "ZeroSector.h"

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
}

void UZeroInputAfternoonComponent::Walk()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void UZeroInputAfternoonComponent::InteractBeam()
{
	FVector EyeVectorStart;
	FRotator EyeRotatorStart;
	Player->GetController()->GetPlayerViewPoint(EyeVectorStart, EyeRotatorStart);

	FVector EyeVectorEnd = EyeVectorStart + EyeRotatorStart.Vector() * DetectDistance;
	FHitResult HitResult;
	FCollisionQueryParams Param(NAME_None, false, GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeVectorStart, EyeVectorEnd, ECC_GameTraceChannel1, Param);
	InteractProcess(HitResult, bHit);
}

void UZeroInputAfternoonComponent::InteractProcess(const FHitResult& InHitResult, bool bIsHit)
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
		IZeroAfternoonInputInterface* Interface = Cast<IZeroAfternoonInputInterface>(Player);
		if (Interface)
		{
			Interface->CloseInteractUI();
		}
	}
	//DrawDebugLine(GetWorld(), EyeVectorStart, EyeVectorEnd, Color, false);
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

void UZeroInputAfternoonComponent::ProvisoInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("Proviso")))
	{
		OnProvisoInteract.ExecuteIfBound();
	}
}

void UZeroInputAfternoonComponent::OperationBoardInteract()
{
	if (InteractedGimmick && InteractedGimmick->ActorHasTag(TEXT("OperationBoard")))
	{
		ZE_LOG(LogZeroSector, Display, TEXT("OperationBoard Interact"));
		OnOperationInteract.ExecuteIfBound();
	}
}

void UZeroInputAfternoonComponent::ToggleNote()
{
	OnNoteDisplay.ExecuteIfBound();
}

void UZeroInputAfternoonComponent::BeginPlay()
{
	Super::BeginPlay();
	
	check(Player);
}

void UZeroInputAfternoonComponent::SetDefaultMovement()
{
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UZeroInputAfternoonComponent::SetDialogueMovement()
{
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}
