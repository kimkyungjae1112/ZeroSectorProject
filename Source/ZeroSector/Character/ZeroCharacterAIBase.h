// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "Interface/ZeroCharacterAIInterface.h"
#include "ZeroCharacterAIBase.generated.h"

UCLASS(abstract)
class ZEROSECTOR_API AZeroCharacterAIBase 
	: public AZeroCharacterBase
	, public IZeroCharacterAIInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterAIBase();

	/* IZeroCharacterAIInterface Implement */
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FOnAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

protected:
	virtual void BeginPlay() override;

protected:
	FOnAttackFinished OnAttackFinished;

};
