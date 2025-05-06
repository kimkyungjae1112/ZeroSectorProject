// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_IsHit.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsHit::UBTDecorator_IsHit()
{
	NodeName = TEXT("Is Hit");
}

bool UBTDecorator_IsHit::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	return OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("IsHit"));
}