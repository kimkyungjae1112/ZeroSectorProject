// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "ZeroCharacterRangedZombie.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterRangedZombie : public AZeroCharacterBaseZombie
{
	GENERATED_BODY()

public:
	AZeroCharacterRangedZombie();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
