// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroWeaponBase.generated.h"

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
	UPROPERTY(EditDefaultsOnly, Category = "Stat")
	float MaxRange;


private:
	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
	/* 
		SoundComp
		ParticleComp
		MeshComp
	*/

};
