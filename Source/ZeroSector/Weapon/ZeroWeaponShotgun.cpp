// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponShotgun.h"

AZeroWeaponShotgun::AZeroWeaponShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/FPS_Weapon_Bundle/Weapons/Meshes/KA_Val/SK_KA_Val_X.SK_KA_Val_X'"));
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
