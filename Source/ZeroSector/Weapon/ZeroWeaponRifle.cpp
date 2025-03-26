// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponRifle.h"
#include "Data/ZeroWeaponStatData.h"

AZeroWeaponRifle::AZeroWeaponRifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/TrueFPS_Asset/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4_X.SK_AR4_X'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UZeroWeaponStatData> StatDataRef(TEXT("/Script/ZeroSector.ZeroWeaponStatData'/Game/Data/WeaponStat/DA_RifleBaseStat.DA_RifleBaseStat'"));
	if (StatDataRef.Object)
	{
		MaxRange = StatDataRef.Object->MaxRange;
		Damage = StatDataRef.Object->Damage;
		FireRate = StatDataRef.Object->FireRate;
		RecoilRate = StatDataRef.Object->RecoilRate;
		DispersionRate = StatDataRef.Object->DispersionRate;
		MaxAmmo = StatDataRef.Object->MaxAmmo;
		CurrentAmmo = MaxAmmo;
	}
}

void AZeroWeaponRifle::Fire()
{
	Super::Fire();

}

void AZeroWeaponRifle::Aiming()
{
	Super::Aiming();

}
