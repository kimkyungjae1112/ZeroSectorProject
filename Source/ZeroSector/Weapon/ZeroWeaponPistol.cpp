// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponPistol.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

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
	static ConstructorHelpers::FObjectFinder<USoundWave> FireSoundRef(TEXT("/Script/Engine.SoundWave'/Game/Characters/Weapons/Assets/Audio/GunFire/SW_GunFire_03.SW_GunFire_03'"));
	if (FireSoundRef.Object)
	{
		FireSound = FireSoundRef.Object;
	}

	GunMeshComp->SetRelativeScale3D(FVector(1.25f, 1.25f, 1.25f));
	WeaponType = EWeaponType::EPistol;
}

void AZeroWeaponPistol::Fire()
{
	Super::Fire();

	AudioComp->SetSound(FireSound);
	AudioComp->Play();
}

