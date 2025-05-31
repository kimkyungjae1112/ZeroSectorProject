// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MeleeBehaviorChoice.generated.h"

UCLASS()
class ZEROSECTOR_API UBTService_MeleeBehaviorChoice : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_MeleeBehaviorChoice();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
