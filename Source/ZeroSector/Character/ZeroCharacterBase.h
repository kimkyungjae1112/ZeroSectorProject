// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "ZeroCharacterBase.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterBase 
	: public ACharacter
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterBase();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;

	FGenericTeamId TeamId;
};
