// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZeroSoundManager.generated.h"

class USoundBase;

UCLASS()
class ZEROSECTOR_API UZeroSoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<USoundBase> PauseSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ProvisoInteractSFX; 

	UPROPERTY()
	TObjectPtr<USoundBase> UIClickSFX;
	
	UPROPERTY()
	TObjectPtr<USoundBase> NoteSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> OperationSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ExcludeSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> SelectSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> EnforceSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> PistolFireSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> RifleFireSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ShotgunFireSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ReloadSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ChangeWeaponSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> Ammo0SFX;

	UPROPERTY()
	TObjectPtr<USoundBase> TriggerSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> FootstepSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ZombieBiteSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ZombieMoveSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ZombieDieSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> NPCInteractSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> WinSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> LoseSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> BossRushAttackSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> BossSpawnAttackSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> PencilSFX;

	void Init();
};
