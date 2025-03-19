// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponPistol.h"
#include "ZeroSector.h"

AZeroWeaponPistol::AZeroWeaponPistol()
{
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
