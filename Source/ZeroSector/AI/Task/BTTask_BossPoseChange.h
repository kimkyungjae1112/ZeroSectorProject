// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_BossPoseChange.generated.h"

class ACharacter;
class UZeroAnimInstanceZombie;

UCLASS()
class ZEROSECTOR_API UBTTask_BossPoseChange : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_BossPoseChange();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> Owner;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UZeroAnimInstanceZombie> ZombieAnim;
};
