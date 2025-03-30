// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/ZeroAIControllerBase.h"
#include "ZeroAIControllerMeleeZombie.generated.h"

struct FAIStimulus;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

UCLASS()
class ZEROSECTOR_API AZeroAIControllerMeleeZombie : public AZeroAIControllerBase
{
	GENERATED_BODY()
	
public:
	AZeroAIControllerMeleeZombie();

	virtual bool IsDead() const override;

protected:
	virtual void BeginPlay() override;
	

private:
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

	void HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus);
	void HandleSenseDamage(AActor* Actor, const FAIStimulus& AIStimulus);

private:
	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	TObjectPtr<UAISenseConfig_Damage> DamageConfig;
};
