// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponPistol.h"
#include "Animation/AnimInstance.h"

AZeroWeaponPistol::AZeroWeaponPistol()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/Assets/Pistols/01/SKM_Modern_Weapons_Pistol_01.SKM_Modern_Weapons_Pistol_01'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> PistolABPRef(TEXT("/Game/Animation/Weapon/Pistol/ABP_Pistol.ABP_Pistol_C"));
	if (PistolABPRef.Class)
	{
		GunMeshComp->SetAnimInstanceClass(PistolABPRef.Class);
	}
	GunMeshComp->SetRelativeScale3D(FVector(1.25f, 1.25f, 1.25f));
	WeaponType = EWeaponType::EPistol;
}

void AZeroWeaponPistol::Fire()
{
	Super::Fire();

}

