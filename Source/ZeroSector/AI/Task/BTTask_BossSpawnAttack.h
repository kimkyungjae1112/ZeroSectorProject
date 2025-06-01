// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossSpawnAttack.generated.h"

UCLASS()
class ZEROSECTOR_API UBTTask_BossSpawnAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossSpawnAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
