// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PlayerController.h"
#include "Character/ZeroCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZeroSector.h"

AZeroWeaponBase::AZeroWeaponBase()
{
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh Component"));
	RootComponent = GunMeshComp;
	GunMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void AZeroWeaponBase::Fire()
{
	if (bIsFire || CurrentAmmo <= 0) return;
	bIsFire = true;

	CurrentAmmo -= 1;
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);

	FTimerHandle FireRateTimer;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AZeroWeaponBase::StopFire, FireRate, false);

	FHitResult HitResult;
	FVector ShotDirection;
	ApplyRecoil();

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

void AZeroWeaponBase::ReloadingCurrentAmmo()
{
	CurrentAmmo = MaxAmmo;
	GunAmmoTextDisplay();
}

void AZeroWeaponBase::GunAmmoTextDisplay()
{
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	OnSetMaxAmmo.ExecuteIfBound(MaxAmmo);
}

void AZeroWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AZeroWeaponBase::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	APlayerController* PC = Cast<APlayerController>(GetOwnerController());
	if (PC == nullptr) return false;

	UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
	FVector2D ViewportSize;
	GameViewport->GetViewportSize(ViewportSize);
	ViewportSize /= 2.f;

	// 크로스헤어 위치를 월드 공간의 방향으로 변환
	FVector CrosshairWorldLocation;
	FVector CrosshairWorldDirection;
	if (!PC->DeprojectScreenPositionToWorld(ViewportSize.X, ViewportSize.Y, CrosshairWorldLocation, CrosshairWorldDirection)) return false;

	FVector Muzzle = GunMeshComp->GetSocketLocation(TEXT("muzzle_Socket"));
	FVector MuzzleEnd = Muzzle + CrosshairWorldDirection * MaxRange;
	ShotDirection = -MuzzleEnd;
	FCollisionQueryParams Params(NAME_None, false, GetOwner());
	
	DrawDebugLine(GetOwner()->GetWorld(), Muzzle, MuzzleEnd, FColor::Red, false, 5.f);
	return GetWorld()->LineTraceSingleByChannel(Hit, Muzzle, MuzzleEnd, ECollisionChannel::ECC_GameTraceChannel3, Params);
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

void AZeroWeaponBase::ApplyRecoil()
{
	if (GetOwnerController())
	{
		FRotator ControlRotation = GetOwnerController()->GetControlRotation();

		float RecoilPitch = FMath::RandRange(-3.f, 3.f);
		float RecoilYaw = FMath::RandRange(-3.f, 3.f);
		ControlRotation.Pitch += RecoilPitch;
		ControlRotation.Yaw += RecoilYaw;

		GetOwnerController()->SetControlRotation(ControlRotation);
	}
}
