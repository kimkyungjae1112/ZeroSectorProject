// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_RangedBehaviorChoice.generated.h"

UCLASS()
class ZEROSECTOR_API UBTService_RangedBehaviorChoice : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_RangedBehaviorChoice();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
