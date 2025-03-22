// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Death.generated.h"

/**
 * 
 */
UCLASS()
class ZEROSECTOR_API UBTTask_Death : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Death();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};