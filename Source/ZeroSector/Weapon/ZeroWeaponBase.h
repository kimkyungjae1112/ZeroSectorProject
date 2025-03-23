// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroWeaponBase.generated.h"

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

	virtual void Fire();
	virtual void Aiming();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> GunMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	float MaxRange;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	float FireRate;

private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	void StopFire();

private:
	bool bIsFire = false;
	
	
	
	
	
	/* 
		SoundComp
		ParticleComp
		MeshComp
	*/

};
