// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "ZeroSector.h"

AZeroWeaponBase::AZeroWeaponBase()
{
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh Component"));
	RootComponent = GunMeshComp;
	GunMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	FireRate = 0.2f;
}

void AZeroWeaponBase::Fire()
{
	if (bIsFire) return;
	bIsFire = true;

	FTimerHandle FireRateTimer;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AZeroWeaponBase::StopFire, FireRate, false);

	FHitResult HitResult;
	FVector ShotDirection;
	bool Hit = GunTrace(HitResult, ShotDirection);
	if (Hit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			ZE_LOG(LogZeroSector, Display, TEXT("Actor Name : %s"), *HitActor->GetActorNameOrLabel());
		}
	}
}

void AZeroWeaponBase::Aiming()
{
	//Aiming 관련 기능 구현
}

void AZeroWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AZeroWeaponBase::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * 2000.f; //MaxRange;
	FCollisionQueryParams Params(NAME_None, false, GetOwner());

	DrawDebugLine(GetOwner()->GetWorld(), Location, End, FColor::Red, false, 5.f);
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel3, Params);
}

AController* AZeroWeaponBase::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn) return OwnerPawn->GetController();
	return nullptr;
}

void AZeroWeaponBase::StopFire()
{
	bIsFire = false;
}


