// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponPistol.h"
#include "ZeroSector.h"

AZeroWeaponPistol::AZeroWeaponPistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FreeFPS/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_Nostock_X.SK_SMG11_Nostock_X'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
}

void AZeroWeaponPistol::Fire()
{
	Super::Fire();

	ZE_LOG(LogZeroSector, Display, TEXT("Pistol Fire"));
}

void AZeroWeaponPistol::Aiming()
{
	Super::Aiming();

}
