// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroWeaponStatData.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroWeaponStatData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Type")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxRange;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float RecoilRate;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float DispersionRate;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 Magazine;
};
