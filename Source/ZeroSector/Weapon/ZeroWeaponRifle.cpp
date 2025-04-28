// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponRifle.h"

AZeroWeaponRifle::AZeroWeaponRifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4_X.SK_AR4_X'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}

	WeaponType = EWeaponType::ERifle;
}

void AZeroWeaponRifle::Fire()
{
	Super::Fire();

}
