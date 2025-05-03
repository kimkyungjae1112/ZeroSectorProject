// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_MoveSpeed.generated.h"

UCLASS()
class ZEROSECTOR_API UBTService_MoveSpeed : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_MoveSpeed();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
