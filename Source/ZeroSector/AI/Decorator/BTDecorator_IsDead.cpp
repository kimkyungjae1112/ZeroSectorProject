// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_IsDead.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsDead::UBTDecorator_IsDead()
{
	NodeName = TEXT("Is Dead");
}

bool UBTDecorator_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool SuperResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	const auto* BB = OwnerComp.GetBlackboardComponent();
	return BB && BB->GetValueAsBool(IsDeadKey);
}