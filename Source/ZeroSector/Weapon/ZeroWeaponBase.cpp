// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "Player/ZeroPlayerController.h"
#include "Character/ZeroCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/ZeroWeaponStatDataTable.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraShakeBase.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ZeroSector.h"

AZeroWeaponBase::AZeroWeaponBase()
{
	GunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh Component"));
	RootComponent = GunMeshComp;
	GunMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	EffectComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Effect Mesh Component"));
	EffectComp->SetupAttachment(GunMeshComp, TEXT("muzzle"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> EffectRef(TEXT("/Script/Engine.StaticMesh'/Game/Characters/Weapons/Assets/VFX/SM_MuzzleFlash_01.SM_MuzzleFlash_01'"));
	if (EffectRef.Object)
	{
		EffectComp->SetStaticMesh(EffectRef.Object);
		EffectComp->SetCollisionProfileName(TEXT("NoCollision"));
		EffectComp->SetVisibility(false);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> PistolDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponStat/PistolStatDataTable.PistolStatDataTable'"));
	if (PistolDataTableRef.Object)
	{
		DataTableBuffer.Add(EWeaponType::EPistol, PistolDataTableRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> RifleDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponStat/RifleStatDataTable.RifleStatDataTable'"));
	if (RifleDataTableRef.Object)
	{
		DataTableBuffer.Add(EWeaponType::ERifle, RifleDataTableRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> ShotgunDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/WeaponStat/ShotgunStatDataTable.ShotgunStatDataTable'"));
	if (ShotgunDataTableRef.Object)
	{
		DataTableBuffer.Add(EWeaponType::EShotgun, ShotgunDataTableRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> MoveTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/ZeroWeaponAnimDataTable.ZeroWeaponAnimDataTable'"));
	if (MoveTableRef.Object)
	{
		MoveTable = MoveTableRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> HitEffectRef(TEXT("/Script/Niagara.NiagaraSystem'/Game/Blood_VFX_Pack/Particles/Systems/P_BigSplash.P_BigSplash'"));
	if (HitEffectRef.Object)
	{
		HitEffect = HitEffectRef.Object;
	}
}

void AZeroWeaponBase::Fire()
{
	if (bIsFire) return;
	bIsFire = true;

	if (CurrentAmmo <= 0)
	{
		Ammo0SoundPlay();
		StartFireTimer();
		return;
	}

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
		break;
	default:
		return;
	}

}

void AZeroWeaponBase::ReloadingCurrentAmmo()
{							
	Anim->Montage_Play(GetReloadingMontage());

	int AddAmmoAmount = Magazine - CurrentAmmo;

	CurrentAmmo = (MaxAmmo - AddAmmoAmount) >= 0 ? CurrentAmmo + AddAmmoAmount : CurrentAmmo + MaxAmmo;
	MaxAmmo -= AddAmmoAmount;
	if (MaxAmmo <= 0) MaxAmmo = 0;
	GunAmmoTextDisplay();
	ReloadSoundPlay();
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
	FZeroWeaponStatDataTable StatDataTable = *DataTableBuffer[WeaponType]->FindRow<FZeroWeaponStatDataTable>(RowIndex, FString());
	MaxRange = StatDataTable.MaxRange;
	Damage = StatDataTable.Damage;
	FireRate = StatDataTable.FireRate;
	RecoilRate = StatDataTable.RecoilRate;
	Spread = StatDataTable.Spread;
	MaxAmmo = StatDataTable.MaxAmmo;
	Magazine = StatDataTable.Magazine;
	CurrentAmmo = Magazine;
}

void AZeroWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<UZeroGameInstance>(GetGameInstance());

	switch (WeaponType)
	{
	case EWeaponType::EPistol:
		MontageData = *MoveTable->FindRow<FZeroWeaponAnimDataTable>(TEXT("Pistol"), FString());
		break;
	case EWeaponType::ERifle:
		MontageData = *MoveTable->FindRow<FZeroWeaponAnimDataTable>(TEXT("Rifle"), FString());
		break;
	case EWeaponType::EShotgun:
		MontageData = *MoveTable->FindRow<FZeroWeaponAnimDataTable>(TEXT("Shotgun"), FString());
		break;
	default:
		ZE_LOG(LogZeroSector, Display, TEXT("무기 애니메이션 안들어옴"));
		return;
	}

	Anim = GunMeshComp->GetAnimInstance();
	ensure(Anim);
}	

bool AZeroWeaponBase::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	FVector CrosshairWorldDirection;
	CalCrosshairVector(CrosshairWorldDirection);

	FVector Muzzle = GunMeshComp->GetSocketLocation(TEXT("muzzle"));
	FVector MuzzleEnd = Muzzle + CrosshairWorldDirection * MaxRange;

	ShotDirection = MuzzleEnd;
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

void AZeroWeaponBase::StartEffectTimer()
{
	FTimerHandle EffectTimer;
	GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &AZeroWeaponBase::StopEffect, 0.1f, false);
}

void AZeroWeaponBase::StopFire()
{
	bIsFire = false;
}

void AZeroWeaponBase::StopEffect()
{
	EffectComp->SetVisibility(false);
}

void AZeroWeaponBase::ApplyRecoil()
{
	if (GetOwnerController())
	{
		FRotator ControlRotation = GetOwnerController()->GetControlRotation();

		float RecoilPitch = FMath::RandRange(-RecoilRate, RecoilRate);
		float RecoilYaw = FMath::RandRange(-RecoilRate, RecoilRate);
		ControlRotation.Pitch += RecoilPitch;
		ControlRotation.Yaw += RecoilYaw;

		GetOwnerController()->SetControlRotation(ControlRotation);
	}
}

void AZeroWeaponBase::PistolFire()
{
	PartialFire();
	PistolFireSoundPlay();

	FHitResult HitResult;
	FVector ShotDirection;

	bool Hit = GunTrace(HitResult, ShotDirection);
	HitCheck(HitResult, ShotDirection, Hit);
}

void AZeroWeaponBase::RifleFire()
{
	PartialFire();
	RifleFireSoundPlay();

	FHitResult HitResult;
	FVector ShotDirection;

	bool Hit = GunTrace(HitResult, ShotDirection);
	HitCheck(HitResult, ShotDirection, Hit);
}

void AZeroWeaponBase::ShotgunFire()
{
	PartialFire();
	ShotgunFireSoundPlay();

	FVector CrosshairWorldDirection;
	CalCrosshairVector(CrosshairWorldDirection);

	FVector Muzzle = GunMeshComp->GetSocketLocation(TEXT("muzzle"));
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

		FVector TraceEnd = Muzzle + SpreadDir.GetSafeNormal() * MaxRange;
		FHitResult HitResult;
		bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Muzzle, TraceEnd, ECollisionChannel::ECC_GameTraceChannel3, Params);
		HitCheck(HitResult, -TraceEnd, Hit);
	}
}

void AZeroWeaponBase::PartialFire()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetOwnerController());
	if (PC == nullptr) return;
	PC->CrosshairSpread(Spread);

	Anim->Montage_Play(GetFireMontage());
	EffectComp->SetVisibility(true);
	StartEffectTimer();

	CurrentAmmo -= 1;
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	ApplyRecoil();
	StartFireTimer();
}

void AZeroWeaponBase::CalCrosshairVector(FVector& CrosshairWorldDirection)
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetOwnerController());
	if (PC == nullptr) return;

	UGameViewportClient* GameViewport = GetWorld()->GetGameViewport();
	FVector2D ViewportSize;
	GameViewport->GetViewportSize(ViewportSize);
	ViewportSize /= 2.f;

	float PixelSpreadRange = PC->GetCurrentSpread() * 0.8f;
	ViewportSize.X += FMath::FRandRange(-PixelSpreadRange, PixelSpreadRange);
	ViewportSize.Y += FMath::FRandRange(-PixelSpreadRange, PixelSpreadRange);

	// 크로스헤어 위치를 월드 공간의 방향으로 변환
	FVector CrosshairWorldLocation;
	if (!PC->DeprojectScreenPositionToWorld(ViewportSize.X, ViewportSize.Y, CrosshairWorldLocation, CrosshairWorldDirection)) return;
}

void AZeroWeaponBase::HitCheck(const FHitResult& InHitResult, const FVector& TraceDir, bool bHit)
{
	if (bHit)
	{
		AActor* HitActor = InHitResult.GetActor();
		if (HitActor)
		{
			HitCheckTimer();
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, InHitResult.ImpactPoint, TraceDir.Rotation());

			FPointDamageEvent DamageEvent(Damage, InHitResult, TraceDir, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			//ZE_LOG(LogZeroSector, Display, TEXT("Actor Name : %s"), *HitActor->GetActorNameOrLabel());
		}
	}
}

void AZeroWeaponBase::HitCheckTimer()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetOwnerController());
	if (PC == nullptr) return;
	PC->HitCrosshair();
	
	FTimerHandle HitCrosshairTimer;
	GetWorld()->GetTimerManager().SetTimer(HitCrosshairTimer, this, &AZeroWeaponBase::UnHitCheck, 0.2f, false);
}

void AZeroWeaponBase::UnHitCheck()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetOwnerController());
	if (PC == nullptr) return;

	PC->UnHitCrosshair();
}

UAnimMontage* AZeroWeaponBase::GetFireMontage() const
{
	if (MontageData.FireMontage.IsPending())
	{
		MontageData.FireMontage.LoadSynchronous();
	}
	return MontageData.FireMontage.Get();
}

UAnimMontage* AZeroWeaponBase::GetReloadingMontage() const
{
	if (MontageData.ReloadingMontage.IsPending())
	{
		MontageData.ReloadingMontage.LoadSynchronous();
	}
	return MontageData.ReloadingMontage.Get();
}

void AZeroWeaponBase::PistolFireSoundPlay() const
{
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->PistolFireSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->PistolFireSFX);
	}
}

void AZeroWeaponBase::RifleFireSoundPlay() const
{
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->RifleFireSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->RifleFireSFX);
	}
}

void AZeroWeaponBase::ShotgunFireSoundPlay() const
{
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ShotgunFireSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->ShotgunFireSFX);
	}
}

void AZeroWeaponBase::Ammo0SoundPlay() const
{
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->Ammo0SFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->Ammo0SFX);
	}
}

void AZeroWeaponBase::ReloadSoundPlay() const
{
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ReloadSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->ReloadSFX);
	}
}

