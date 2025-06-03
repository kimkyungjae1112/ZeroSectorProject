// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_RangedBehaviorChoice.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RangedBehaviorChoice::UBTService_RangedBehaviorChoice()
{
	NodeName = TEXT("Ranged Behavior Choice");
	Interval = 1.5f;
}

void UBTService_RangedBehaviorChoice::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return;

	
	
	// 0 >= / < 100
	int32 Probability = FMath::RandRange(0, 100);

	if (Probability >= 0 && Probability < 70)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsMove"), true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsSpawnAttack"), false);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsMove"), false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsSpawnAttack"), true);
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("bIsSpawnAttack"), true);
}
