// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponRifle.h"

AZeroWeaponRifle::AZeroWeaponRifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Rifle/Mesh/SK_Rifle.SK_Rifle'"));
	if (MeshRef.Object)
	{
		MeshComp->SetSkeletalMesh(MeshRef.Object);
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
