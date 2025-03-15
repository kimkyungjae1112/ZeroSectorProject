// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterAIBase.h"
#include "ZeroCharacterAIZombieTest.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterAIZombieTest : public AZeroCharacterAIBase
{
	GENERATED_BODY()
	
public:
	AZeroCharacterAIZombieTest();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

};
