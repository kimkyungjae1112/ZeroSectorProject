// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ZeroWeaponBase.h"
#include "ZeroWeaponRifle.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroWeaponRifle : public AZeroWeaponBase
{
	GENERATED_BODY()
	
public:
	AZeroWeaponRifle();

	virtual void Fire() override;
	virtual void Aiming() override;
};
