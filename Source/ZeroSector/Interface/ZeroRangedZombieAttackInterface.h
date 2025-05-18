// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroRangedZombieAttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UZeroRangedZombieAttackInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroRangedZombieAttackInterface
{
	GENERATED_BODY()

public:
	virtual void SpawnProjectile() = 0;
};
