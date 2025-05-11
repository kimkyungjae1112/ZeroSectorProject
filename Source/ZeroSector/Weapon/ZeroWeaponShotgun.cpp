// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponShotgun.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

AZeroWeaponShotgun::AZeroWeaponShotgun()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/Assets/Shotguns/02/SKM_Modern_Weapons_Shotgun_02.SKM_Modern_Weapons_Shotgun_02'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> ShotgunABPRef(TEXT("/Game/Animation/Weapon/Shotgun/ABP_Shotgun.ABP_Shotgun_C"));
	if (ShotgunABPRef.Class)
	{
		GunMeshComp->SetAnimInstanceClass(ShotgunABPRef.Class);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> FireSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Characters/Weapons/Assets/Audio/GunFire/SW_GunFire_01.SW_GunFire_01'"));
	if (FireSoundRef.Object)
	{
		FireSound = FireSoundRef.Object;
	}

	WeaponType = EWeaponType::EShotgun;
}

void AZeroWeaponShotgun::Fire()
{
	Super::Fire();

	AudioComp->SetSound(FireSound);
	AudioComp->Play();
}
