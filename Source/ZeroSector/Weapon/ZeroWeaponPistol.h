// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ZeroWeaponBase.h"
#include "ZeroWeaponPistol.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroWeaponPistol : public AZeroWeaponBase
{
	GENERATED_BODY()
	
public:
	AZeroWeaponPistol();

	virtual void Fire() override;
	virtual FORCEINLINE void Upgrade(int32 InLevel) override { Level = InLevel; }
	virtual FORCEINLINE int32 GetLevel() const override { return Level; }
};
