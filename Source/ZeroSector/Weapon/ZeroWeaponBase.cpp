// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ZeroWeaponBase.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/PlayerController.h"
#include "Character/ZeroCharacterPlayer.h"
#include "Kismet/KismetMathLibrary.h"
#include "Data/ZeroWeaponStatDataTable.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraShakeBase.h"
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

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> PistolShakeRef(TEXT("/Game/Blueprints/Camera/BP_CameraShakePistol.BP_CameraShakePistol_C"));
	if (PistolShakeRef.Class)
	{
		PistolShake = PistolShakeRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> RifleShakeRef(TEXT("/Game/Blueprints/Camera/BP_CameraShakeRifle.BP_CameraShakeRifle_C"));
	if (RifleShakeRef.Class)
	{
		RifleShake = RifleShakeRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> ShotgunShakeRef(TEXT("/Game/Blueprints/Camera/BP_CameraShakeShotgun.BP_CameraShakeShotgun_C"));
	if (ShotgunShakeRef.Class)
	{
		ShotgunShake = ShotgunShakeRef.Class;
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

void AZeroWeaponBase::StopFire()
{
	EffectComp->SetVisibility(false);
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
	Anim->Montage_Play(GetFireMontage());
	Cast<APlayerController>(GetOwnerController())->ClientStartCameraShake(PistolShake);
	EffectComp->SetVisibility(true);

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
	Anim->Montage_Play(GetFireMontage());
	Cast<APlayerController>(GetOwnerController())->ClientStartCameraShake(RifleShake);
	EffectComp->SetVisibility(true);

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
	Anim->Montage_Play(GetFireMontage());
	Cast<APlayerController>(GetOwnerController())->ClientStartCameraShake(ShotgunShake);
	EffectComp->SetVisibility(true);

	CurrentAmmo -= 1;
	OnChangedAmmo.ExecuteIfBound(CurrentAmmo);
	ApplyRecoil();
	StartFireTimer();

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

