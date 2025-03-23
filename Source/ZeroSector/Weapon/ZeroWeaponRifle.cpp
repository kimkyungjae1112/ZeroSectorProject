// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponRifle.h"

AZeroWeaponRifle::AZeroWeaponRifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/FreeFPS/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
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
