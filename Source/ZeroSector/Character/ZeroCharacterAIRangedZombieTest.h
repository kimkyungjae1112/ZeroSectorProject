// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterAIBase.h"
#include "Interface/ZeroCharacterAIInterface.h"
#include "ZeroCharacterAIRangedZombieTest.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterAIRangedZombieTest : public AZeroCharacterAIBase, public IZeroCharacterAIInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterAIRangedZombieTest();

	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual void ApplyDamage(float Damage) override;

protected:
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	// ���� �� �˸�
	void OnAttackFinishedInternal();

	void FireProjectile();

protected:
	FAICharacterAttackFinished OnAttackFinished;
	FTimerHandle AttackFinishTimerHandle;

};