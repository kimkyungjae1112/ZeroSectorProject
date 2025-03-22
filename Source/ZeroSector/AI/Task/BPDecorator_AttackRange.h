// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BPDecorator_AttackRange.generated.h"

/**
 * 
 */
UCLASS()
class UBPDecorator_AttackRange : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
	
public:
	UBPDecorator_AttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
