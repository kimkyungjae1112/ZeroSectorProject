// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroWeaponBase.generated.h"

class UZeroWeaponAnimInstance;

UENUM()
enum class EFireMode : uint8
{
	ESingle,
	EAuto
};

UCLASS(abstract)
class ZEROSECTOR_API AZeroWeaponBase 
	: public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroWeaponBase();

	virtual void Tick(float DeltaTime) override;
	virtual void Fire();
	virtual void Aiming();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> GunMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float MaxRange;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float Damage;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float FireRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float RecoilRate;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	float DispersionRate;
	
private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	void StopFire();
	void ApplyRecoil();
	//void GunDirectionUpdate(float DeltaTime);



private:
	FTransform WeaponSocketTransform;
	bool bIsFire = false;
	
	
	
	
	
	/* 
		SoundComp
		ParticleComp
		MeshComp
	*/

};
