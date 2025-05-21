// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_MoveSpeed.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ZeroCharacterAIInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"

UBTService_MoveSpeed::UBTService_MoveSpeed()
{
	NodeName = TEXT("Modified Move Speed");
	Interval = 0.5f;
}

void UBTService_MoveSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACharacter* Owner = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr) return;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (Target == nullptr) return;

	IZeroCharacterAIInterface* Interface = Cast<IZeroCharacterAIInterface>(Owner);
	if (Interface == nullptr) return;

	float Dist = FVector::Dist(Owner->GetActorLocation(), Target->GetActorLocation());
	float OwnerRunSpeed = Interface->GetRunSpeed();
	float OwnerWalkSpeed = Interface->GetWalkSpeed();

	const float MinDist = 500.0f; 
	const float MaxDist = 2000.0f; 

	float Alpha = 1.0f - FMath::Clamp((Dist - MinDist) / (MaxDist - MinDist), 0.0f, 1.0f);

	float NewSpeed = FMath::Lerp(OwnerWalkSpeed, OwnerRunSpeed, Alpha);

	if (Owner && Owner->GetCharacterMovement())
	{
		Owner->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

		static float LastGroanTime = 0.f;
		float CurrentTime = Owner->GetWorld()->GetTimeSeconds();

		if (Dist < 1000.f && (CurrentTime - LastGroanTime > 4.f) && FMath::FRand() < 0.3f)
		{
			UZeroGameInstance* GI = Cast<UZeroGameInstance>(Owner->GetGameInstance());
			if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ZombieMoveSFX)
			{
				UGameplayStatics::PlaySoundAtLocation(Owner, GI->GetSoundManager()->ZombieMoveSFX, Owner->GetActorLocation());
				LastGroanTime = CurrentTime;
			}
		}
	}
}
