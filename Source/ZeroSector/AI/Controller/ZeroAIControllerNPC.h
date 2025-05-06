// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZeroAIControllerNPC.generated.h"

class UBlackboardData;
class UBehaviorTree;

USTRUCT()
struct FWayPoint
{
	GENERATED_BODY()

public:
	FWayPoint() {}

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<FVector> WayPoint;
};

UCLASS()
class ZEROSECTOR_API AZeroAIControllerNPC : public AAIController
{
	GENERATED_BODY()
	
public:
	AZeroAIControllerNPC();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void MoveToNextPoint();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BBData;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BTData;

	UPROPERTY(EditAnywhere, Category = "AI")
	TMap<uint8, FWayPoint> WayPoints;

	int32 CurrentIndex{ 0 };
};
