// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SetOffsetLocation.generated.h"

UCLASS()
class ZEROSECTOR_API UBTService_SetOffsetLocation : public UBTService
{
	GENERATED_BODY()

	public:
	UBTService_SetOffsetLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "AI")
	float DesiredDistance = 800.f; 
};