// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_AttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "ZeroHeader/ZeroAIHeader.h"
#include "Interface/ZeroCharacterAIInterface.h"

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

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