// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "ZeroAIControllerBase.generated.h"

class UBlackboardData;
class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;
struct FAIStimulus;

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

	virtual bool IsDead() const;


protected:
	virtual void OnPossess(APawn* InPawn) override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;


public:
	void RunAI();
	void StopAI();


protected:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

	void HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus);
	void HandleSenseDamage(AActor* Actor, const FAIStimulus& AIStimulus);

	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;

protected:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> PerceptionComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBlackboardData> BBData;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BTData;

	FGenericTeamId TeamID;

};
