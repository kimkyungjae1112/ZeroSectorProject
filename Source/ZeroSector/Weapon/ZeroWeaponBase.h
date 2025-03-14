// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroWeaponBase.generated.h"

UCLASS()
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

private:


	/* 
		SoundComp
		ParticleComp
		MeshComp
	*/

};
