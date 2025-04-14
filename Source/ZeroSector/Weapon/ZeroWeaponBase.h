// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroWeaponBase.generated.h"

class UZeroWeaponAnimInstance;

DECLARE_DELEGATE_OneParam(FOnChangedAmmo, int32 /* Current Ammo */)
DECLARE_DELEGATE_OneParam(FOnSetMaxAmmo, int32 /* Max Ammo */)

UCLASS(abstract)
class ZEROSECTOR_API AZeroWeaponBase 
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroWeaponBase();

	virtual void Fire();
	virtual FORCEINLINE void Upgrade(int32 InLevel) {}
	virtual FORCEINLINE int32 GetLevel() const { return Level; }

public:
	FORCEINLINE USkeletalMeshComponent* GetGunMeshComp() const { return GunMeshComp; }

	void ReloadingCurrentAmmo();
	void GunAmmoTextDisplay();
	void StatApply();

	FOnChangedAmmo OnChangedAmmo;
	FOnSetMaxAmmo OnSetMaxAmmo;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Type")
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float MaxRange;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float FireRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float RecoilRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 MaxAmmo;
	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 Magazine;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 CurrentAmmo;

	UPROPERTY()
	TMap<EWeaponType, UDataTable*> DataTableBuffer;

	int32 Level = 1;

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
	int32 MaxLevel = 7;

	bool bIsFire = false;
	
	
	
	/* 
		SoundComp
		ParticleComp
		MeshComp
	*/

};
