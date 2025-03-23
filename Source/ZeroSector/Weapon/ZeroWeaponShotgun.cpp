// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponShotgun.h"

AZeroWeaponShotgun::AZeroWeaponShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Shotgun/Mesh/SKM_Shotgun.SKM_Shotgun'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
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
