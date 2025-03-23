// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/ZeroAIControllerBase.h"
#include "ZeroAIControllerRangedZombie.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

/**
 * 
 */
UCLASS()
class ZEROSECTOR_API AZeroAIControllerRangedZombie : public AZeroAIControllerBase
{
	GENERATED_BODY()

	public:
	AZeroAIControllerRangedZombie();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	class UAISenseConfig_Sight* SightConfig;
	class UAISenseConfig_Damage* DamageConfig;

	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);
	void HandleSenseSight(AActor* Actor, const FAIStimulus& Stimulus);
	void HandleSenseDamage(AActor* Actor, const FAIStimulus& Stimulus);
};