// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponPistol.h"
#include "Data/ZeroWeaponStatData.h"
#include "ZeroSector.h"

AZeroWeaponPistol::AZeroWeaponPistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/TrueFPS_Asset/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Nostock_X.SK_SMG11_Nostock_X'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UZeroWeaponStatData> StatDataRef(TEXT("/Script/ZeroSector.ZeroWeaponStatData'/Game/Data/WeaponStat/DA_PistolBaseStat.DA_PistolBaseStat'"));
	if (StatDataRef.Object)
	{
		MaxRange = StatDataRef.Object->MaxRange;
		Damage = StatDataRef.Object->Damage;
		FireRate = StatDataRef.Object->FireRate;
		RecoilRate = StatDataRef.Object->RecoilRate;
		DispersionRate = StatDataRef.Object->DispersionRate;
		MaxAmmo = StatDataRef.Object->MaxAmmo;
		Magazine = StatDataRef.Object->Magazine;
		CurrentAmmo = Magazine;
	}

	WeaponType = EWeaponType::EPistol;
}

void AZeroWeaponPistol::Fire()
{
	Super::Fire();

}

