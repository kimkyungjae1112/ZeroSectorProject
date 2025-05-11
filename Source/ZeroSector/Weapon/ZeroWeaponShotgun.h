// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ZeroWeaponBase.h"
#include "ZeroWeaponShotgun.generated.h"

class USoundWave;

UCLASS()
class ZEROSECTOR_API AZeroWeaponShotgun : public AZeroWeaponBase
{
	GENERATED_BODY()
	
public:
	AZeroWeaponShotgun();

	virtual void Fire() override;
	virtual FORCEINLINE void Upgrade(int32 InLevel) override { Level = InLevel; }
	virtual FORCEINLINE int32 GetLevel() const override { return Level; }

private:
	UPROPERTY(VisibleAnywhere, Category = "Sound")
	TObjectPtr<USoundWave> FireSound;
};
