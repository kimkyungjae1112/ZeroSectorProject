// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponShotgun.h"

AZeroWeaponShotgun::AZeroWeaponShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/Assets/Shotguns/02/SKM_Modern_Weapons_Shotgun_02.SKM_Modern_Weapons_Shotgun_02'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}

	WeaponType = EWeaponType::EShotgun;
}

void AZeroWeaponShotgun::Fire()
{
	Super::Fire();

}
