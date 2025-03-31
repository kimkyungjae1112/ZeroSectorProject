// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroZombieSpawner.generated.h"

class USplineComponent;
class AZeroCharacterAIMeleeZombie;

UCLASS()
class ZEROSECTOR_API AZeroZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroZombieSpawner();

protected:
	virtual void BeginPlay() override;

public:
	void SpawnZombie();

private:
	UPROPERTY(VisibleAnywhere, Category = "Zero")
	TObjectPtr<USplineComponent> SplineComp;

	UPROPERTY(EditDefaultsOnly, Category = "Zero")
	TSubclassOf<AZeroCharacterAIMeleeZombie> MeleeZombieClass;

	UPROPERTY(EditAnywhere, Category = "Zero")
	int ZombieNum = 5;

	UPROPERTY(EditAnywhere, Category = "Zero")
	float Spacing = 200.f;

};
