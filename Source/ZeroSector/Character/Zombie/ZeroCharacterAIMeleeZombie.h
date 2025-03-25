// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterAIBase.h"
#include "ZeroCharacterAIMeleeZombie.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterAIMeleeZombie : public AZeroCharacterAIBase
{
	GENERATED_BODY()
	
public:
	AZeroCharacterAIMeleeZombie();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;
};
