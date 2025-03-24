// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponShotgun.h"
#include "Data/ZeroWeaponStatData.h"

AZeroWeaponShotgun::AZeroWeaponShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Shotgun/Mesh/SKM_Shotgun.SKM_Shotgun'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UZeroWeaponStatData> StatDataRef(TEXT("/Script/ZeroSector.ZeroWeaponStatData'/Game/Data/WeaponStat/DA_ShotgunBaseStat.DA_ShotgunBaseStat'"));
	if (StatDataRef.Object)
	{
		MaxRange = StatDataRef.Object->MaxRange;
		Damage = StatDataRef.Object->Damage;
		FireRate = StatDataRef.Object->FireRate;
		RecoilRate = StatDataRef.Object->RecoilRate;
		DispersionRate = StatDataRef.Object->DispersionRate;
	}
}

void AZeroWeaponShotgun::Fire()
{
	Super::Fire();

}

void AZeroWeaponShotgun::Aiming()
{
	Super::Aiming();

}
