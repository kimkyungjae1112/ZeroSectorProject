// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ZeroWeaponBase.h"
#include "ZeroWeaponShotgun.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroWeaponShotgun : public AZeroWeaponBase
{
	GENERATED_BODY()
	
public:
	AZeroWeaponShotgun();

	virtual void Fire() override;
	virtual void Aiming() override;
};
