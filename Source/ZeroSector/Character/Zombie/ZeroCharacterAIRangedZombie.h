// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterAIBase.h"
#include "ZeroCharacterAIRangedZombie.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterAIRangedZombie : public AZeroCharacterAIBase
{
	GENERATED_BODY()

public:
	AZeroCharacterAIRangedZombie();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
