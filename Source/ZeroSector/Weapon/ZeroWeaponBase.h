// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon/ZeroWeaponType.h"
#include "Data/Animation/ZeroWeaponAnimDataTable.h"
#include "ZeroWeaponBase.generated.h"

class UZeroWeaponAnimInstance;
class UAnimMontage;
class UAnimInstance;
class UCameraShakeBase;
class UAudioComponent;
class UZeroGameInstance;
class UNiagaraSystem;

DECLARE_DELEGATE_OneParam(FOnChangedAmmo, int32 /* Current Ammo */)
DECLARE_DELEGATE_OneParam(FOnSetMaxAmmo, int32 /* Max Ammo */)

UCLASS(abstract)
class ZEROSECTOR_API AZeroWeaponBase 
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroWeaponBase();

	virtual void Fire();
	virtual FORCEINLINE void Upgrade(int32 InLevel) {}
	virtual FORCEINLINE int32 GetLevel() const { return Level; }

public:
	FORCEINLINE USkeletalMeshComponent* GetGunMeshComp() const { return GunMeshComp; }
	FORCEINLINE bool IsVaildFire() const { return CurrentAmmo > 0; }
	FORCEINLINE bool IsVaildReload() const { return CurrentAmmo < Magazine; }

	void ReloadingCurrentAmmo();
	void GunAmmoTextDisplay();
	void StatApply();

	FOnChangedAmmo OnChangedAmmo;
	FOnSetMaxAmmo OnSetMaxAmmo;

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Type")
	EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float MaxRange;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float FireRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float RecoilRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float Spread;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 MaxAmmo;
	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 Magazine;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	int32 CurrentAmmo;

	UPROPERTY()
	TMap<EWeaponType, UDataTable*> DataTableBuffer;

	int32 Level = 1;

// 특수효과 컴포넌트
protected:
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UStaticMeshComponent> EffectComp;


// 무기 실제 기능
private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	void StartFireTimer();
	void StartEffectTimer();
	void StopFire();
	void StopEffect();
	void ApplyRecoil();
	

	void PistolFire();
	void RifleFire();
	void ShotgunFire();
	void PartialFire();
	void CalCrosshairVector(FVector& CrosshairWorldDirection);
	void HitCheck(const FHitResult& InHitResult, const FVector& TraceDir, bool bHit);
	void HitCheckTimer();
	void UnHitCheck();

// 기타 데이터
private:
	int32 MaxLevel = 7;
	bool bIsFire{ false };

// 애니메이션 데이터
private:
	// 애셋 경로에서 실제 로딩 도와주는 함수
	UAnimMontage* GetFireMontage() const;
	UAnimMontage* GetReloadingMontage() const;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UDataTable> MoveTable;

	UPROPERTY(VisibleAnywhere, Category = "Anim")
	FZeroWeaponAnimDataTable MontageData;
	

// 특수 효과 ( 화면 흔들림, 이펙트, 사운드 )
private:
	void PistolFireSoundPlay() const;
	void RifleFireSoundPlay() const;
	void ShotgunFireSoundPlay() const;
	void Ammo0SoundPlay() const;
	void ReloadSoundPlay() const;

	UPROPERTY()
	TObjectPtr<UZeroGameInstance> GI;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> HitEffect;

};
