// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "ZeroAIControllerBase.generated.h"

class UBlackboardData;
class UBehaviorTree;
class UAIPerceptionComponent;

UENUM()
enum class EAIPerceptionSense : uint8
{
	EPS_None = 0,
	EPS_Sight,
	EPS_Damage
};

UCLASS(abstract)
class ZEROSECTOR_API AZeroAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AZeroAIControllerBase();

	virtual bool IsDead() const PURE_VIRTUAL(AZeroAIControllerBase::IsDead, return false;);


protected:
	virtual void OnPossess(APawn* InPawn) override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


public:
	void RunAI();
	void StopAI();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBlackboardData> BBData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBehaviorTree> BTData;

	FGenericTeamId TeamID;

};
