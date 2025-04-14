// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponPistol.h"

AZeroWeaponPistol::AZeroWeaponPistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/TrueFPS_Asset/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Nostock_X.SK_SMG11_Nostock_X'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}

	WeaponType = EWeaponType::EPistol;
}

void AZeroWeaponPistol::Fire()
{
	Super::Fire();

}

