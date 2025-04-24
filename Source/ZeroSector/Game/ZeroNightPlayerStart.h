// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroNightPlayerStart.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroNightPlayerStart : public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroNightPlayerStart();

public:
	FORCEINLINE uint8 GetStartDay() const { return StartDay; }
	FORCEINLINE TArray<FVector> GetZombieSpawnerLoc() const { return ZombieSpawnerLoc; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, Category = "Start", meta = (AllowPrivateAccess = "true"))
	uint8 StartDay;

	UPROPERTY(EditAnywhere, Category = "Start", meta = (AllowPrivateAccess = "true"))
	TArray<FVector> ZombieSpawnerLoc;
};
