// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ZeroCharacterAIInterface.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("Attack In Range");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == Owner)
	{
		return false;
	}

	IZeroCharacterAIInterface* Interface = Cast<IZeroCharacterAIInterface>(Owner);
	if (nullptr == Interface)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	if (nullptr == Target)
	{
		return false;
	}

	float Distance = FVector::Dist(Owner->GetActorLocation(), Target->GetActorLocation());
	return Distance < Interface->GetAIAttackRange();
}