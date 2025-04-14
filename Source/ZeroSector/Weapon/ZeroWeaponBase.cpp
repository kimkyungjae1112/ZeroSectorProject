// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PlayerController.h"
#include "Character/ZeroCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/ZeroWeaponStatDataTable.h"
#include "ZeroSector.h"

AZeroWeaponBase::AZeroWeaponBase()
{
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh Component"));
	RootComponent = GunMeshComp;
	GunMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UDataTable> PistolDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponStat/PistolStatDataTable.PistolStatDataTable'"));
	if (PistolDataTableRef.Succeeded())
	{
		DataTableBuffer.Add(EWeaponType::EPistol ,PistolDataTableRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> RifleDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponStat/RifleStatDataTable.RifleStatDataTable'"));
	if (RifleDataTableRef.Succeeded())
	{
		DataTableBuffer.Add(EWeaponType::ERifle ,RifleDataTableRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> ShotgunDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponStat/ShotgunStatDataTable.ShotgunStatDataTable'"));
	if (ShotgunDataTableRef.Succeeded())
	{
		DataTableBuffer.Add(EWeaponType::EShotgun ,ShotgunDataTableRef.Object);
	}
}

void AZeroWeaponBase::Fire()
{
	if (bIsFire || CurrentAmmo <= 0) return;
	bIsFire = true;

	switch (WeaponType)
	{
	case EWeaponType::EPistol:
		PistolFire();
		break;
	case EWeaponType::ERifle:
		RifleFire();
		break;
	case EWeaponType::EShotgun:
		ShotgunFire();
		ZE_LOG(LogZeroSector, Display, TEXT("Shotgun Fire"));
		break;
	default:
		return;
	}

}

void AZeroWeaponBase::ReloadingCurrentAmmo()
{							
	int AddAmmoAmount = Magazine - CurrentAmmo;

	CurrentAmmo = (MaxAmmo - AddAmmoAmount) >= 0 ? CurrentAmmo + AddAmmoAmount : CurrentAmmo + MaxAmmo;
	MaxAmmo -= AddAmmoAmount;
	if (MaxAmmo <= 0) MaxAmmo = 0;
	GunAmmoTextDisplay();
}

void AZeroWeaponBase::GunAmmoTextDisplay()
{
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	OnSetMaxAmmo.ExecuteIfBound(MaxAmmo);
} 

void AZeroWeaponBase::StatApply()
{
	if (Level > MaxLevel) Level = 7;

	FName RowIndex = FName(*FString::Printf(TEXT("%d"), Level));
	FString Context(TEXT("Weapon Stat"));
	FZeroWeaponStatDataTable StatDataTable = *DataTableBuffer[WeaponType]->FindRow<FZeroWeaponStatDataTable>(RowIndex, Context);
	MaxRange = StatDataTable.MaxRange;
	Damage = StatDataTable.Damage;
	FireRate = StatDataTable.FireRate;
	RecoilRate = StatDataTable.RecoilRate;
	MaxAmmo = StatDataTable.MaxAmmo;
	Magazine = StatDataTable.Magazine;
	CurrentAmmo = Magazine;
}

void AZeroWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}	

bool AZeroWeaponBase::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	FVector CrosshairWorldDirection;
	CalCrosshairVector(CrosshairWorldDirection);

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

void AZeroWeaponBase::StartFireTimer()
{
	FTimerHandle FireRateTimer;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimer, this, &AZeroWeaponBase::StopFire, FireRate, false);
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

void AZeroWeaponBase::PistolFire()
{
	CurrentAmmo -= 1;
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	StartFireTimer();

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

void AZeroWeaponBase::RifleFire()
{
	CurrentAmmo -= 1;
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	StartFireTimer();

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

void AZeroWeaponBase::ShotgunFire()
{
	CurrentAmmo -= 1;
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	ApplyRecoil();
	StartFireTimer();

	FVector CrosshairWorldDirection;
	CalCrosshairVector(CrosshairWorldDirection);

	FVector Muzzle = GunMeshComp->GetSocketLocation(TEXT("muzzle_Socket"));
	FCollisionQueryParams Params(NAME_None, false, GetOwner());

	int32 PelletCount = 10;
	float SpreadAngleDeg = 20.0f;
	float SpreadAngleRad = FMath::DegreesToRadians(SpreadAngleDeg);

	for (int i = 0; i < 20; ++i)
	{
		FVector ShootDir = CrosshairWorldDirection.GetSafeNormal();
		FMatrix RotMatrix = FRotationMatrix::MakeFromX(ShootDir); 

		FVector RightVector = RotMatrix.GetScaledAxis(EAxis::Y); 
		FVector UpVector = RotMatrix.GetScaledAxis(EAxis::Z);   

		float SpreadRad = FMath::DegreesToRadians(SpreadAngleDeg);
		float RandomX = FMath::FRandRange(-1.f, 1.f);
		float RandomY = FMath::FRandRange(-1.f, 1.f);

		FVector SpreadDir = ShootDir
			+ RightVector * FMath::Tan(SpreadRad) * RandomX
			+ UpVector * FMath::Tan(SpreadRad) * RandomY;

		SpreadDir = SpreadDir.GetSafeNormal();

		FVector TraceEnd = Muzzle + SpreadDir * MaxRange;

		FHitResult HitResult;
		FColor Color{ FColor::Red };
		bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Muzzle, TraceEnd, ECollisionChannel::ECC_GameTraceChannel3, Params);
		if (Hit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor)
			{
				FPointDamageEvent DamageEvent(Damage, HitResult, -TraceEnd, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
				ZE_LOG(LogZeroSector, Display, TEXT("Actor Name : %s"), *HitActor->GetActorNameOrLabel());
				Color = FColor::Green;
			}
		}
		DrawDebugLine(GetOwner()->GetWorld(), Muzzle, TraceEnd, Color, false, 5.f);
	}
}

void AZeroWeaponBase::CalCrosshairVector(FVector& CrosshairWorldDirection)
{
	APlayerController* PC = Cast<APlayerController>(GetOwnerController());
	if (PC == nullptr) return;

	UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
	FVector2D ViewportSize;
	GameViewport->GetViewportSize(ViewportSize);
	ViewportSize /= 2.f;

	// 크로스헤어 위치를 월드 공간의 방향으로 변환
	FVector CrosshairWorldLocation;
	if (!PC->DeprojectScreenPositionToWorld(ViewportSize.X, ViewportSize.Y, CrosshairWorldLocation, CrosshairWorldDirection)) return;
}

