// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponRifle.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "Kismet/GameplayStatics.h"

AZeroWeaponRifle::AZeroWeaponRifle()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunBodyMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/Assets/Rifles/01/SKM_Modern_Weapons_Rifle_01.SKM_Modern_Weapons_Rifle_01'"));
	if (GunBodyMeshRef.Object)
	{
		GunMeshComp->SetSkeletalMesh(GunBodyMeshRef.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> RifleABPRef(TEXT("/Game/Animation/Weapon/Rifle/ABP_Rifle.ABP_Rifle_C"));
	if (RifleABPRef.Class)
	{
		GunMeshComp->SetAnimInstanceClass(RifleABPRef.Class);
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> FireSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Characters/Weapons/Assets/Audio/GunFire/SW_GunFire_04.SW_GunFire_04'"));
	if (FireSoundRef.Object)
	{
		FireSound = FireSoundRef.Object;
	}

	WeaponType = EWeaponType::ERifle;
}

void AZeroWeaponRifle::Fire()
{
	Super::Fire();

	AudioComp->SetSound(FireSound);
	AudioComp->Play();
}
