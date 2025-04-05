// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "Interface/ZeroCharacterAIInterface.h"
#include "ZeroCharacterBaseZombie.generated.h"

class UZeroStatComponent;

UCLASS(abstract)
class ZEROSECTOR_API AZeroCharacterBaseZombie
	: public AZeroCharacterBase
	, public IZeroCharacterAIInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterBaseZombie();

	/* IZeroCharacterAIInterface Implement */
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackOneDelegate(const FOnAttackOneFinished& InOnAttackOneFinished) override;
	virtual void SetAIAttackTwoDelegate(const FOnAttackOneFinished& InOnAttackTwoFinished) override;
	virtual void AttackOneByAI() override;
	virtual void AttackTwoByAI() override;

	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	TObjectPtr<UZeroStatComponent> StatComp;

protected:
	FOnAttackOneFinished OnAttackOneFinished;
	FOnAttackTwoFinished OnAttackTwoFinished;

};
