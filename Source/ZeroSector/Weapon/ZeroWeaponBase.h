// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroWeaponBase.generated.h"

class UZeroWeaponAnimInstance;

DECLARE_DELEGATE_OneParam(FOnChangedAmmo, uint8 /* Current Ammo */)
DECLARE_DELEGATE_OneParam(FOnSetMaxAmmo, uint8 /* Max Ammo */)

UCLASS(abstract)
class ZEROSECTOR_API AZeroWeaponBase 
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroWeaponBase();

	virtual void Fire();

public:
	FORCEINLINE USkeletalMeshComponent* GetGunMeshComp() const { return GunMeshComp; }

	void ReloadingCurrentAmmo();
	void GunAmmoTextDisplay();

	FOnChangedAmmo OnChangedAmmo;
	FOnSetMaxAmmo OnSetMaxAmmo;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float MaxRange;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float FireRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float RecoilRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float DispersionRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	uint8 MaxAmmo;
	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	uint8 CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Type")
	EWeaponType WeaponType;

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	void StartFireTimer();
	void StopFire();
	void ApplyRecoil();


	void PistolFire();
	void RifleFire();
	void ShotgunFire();
	void CalCrosshairVector(FVector& CrosshairWorldDirection);

private:
	bool bIsFire = false;
	
	
	
	
	
	/* 
		SoundComp
		ParticleComp
		MeshComp
	*/

};
