// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossRushAttack.generated.h"

UCLASS()
class ZEROSECTOR_API UBTTask_BossRushAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossRushAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
