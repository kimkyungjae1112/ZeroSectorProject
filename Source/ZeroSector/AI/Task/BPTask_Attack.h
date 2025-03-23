// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BPTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ZEROSECTOR_API UBPTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBPTask_Attack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};