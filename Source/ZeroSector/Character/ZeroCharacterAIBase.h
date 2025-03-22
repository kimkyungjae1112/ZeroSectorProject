// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "AIController.h"
#include "ZeroCharacterAIBase.generated.h"

UCLASS(abstract)
class ZEROSECTOR_API AZeroCharacterAIBase : public AZeroCharacterBase
{
	GENERATED_BODY()

public:
	AZeroCharacterAIBase();
	virtual void BeginPlay() override;

public:
	virtual void ApplyDamage(float Damage);


protected:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float Health = 100.0f;

	FTimerHandle HitResetTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool bIsRangedZombie;

	bool IsRangedZombie() const { return bIsRangedZombie; }
};
