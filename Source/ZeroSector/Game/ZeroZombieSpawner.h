// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroZombieSpawner.generated.h"

class USplineComponent;
class AZeroCharacterMeleeZombie;
class AZeroCharacterRangedZombie;
class AZeroCharacterBossZombie;

UCLASS()
class ZEROSECTOR_API AZeroZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroZombieSpawner();

	FORCEINLINE uint8 GetStartDay() const { return StartDay; }


protected:
	virtual void BeginPlay() override;

public:
	void SpawnZombie(uint8 CommonZombieNum, uint8 RangedZombieNum, uint8 MiniZombieNum, uint8 TankerZombieNum, uint8 BossZombieNum);

private:
	UPROPERTY(VisibleAnywhere, Category = "Zero")
	TObjectPtr<USplineComponent> SplineComp;

	UPROPERTY(EditDefaultsOnly, Category = "Zero")
	TArray<TSubclassOf<AZeroCharacterMeleeZombie>> MeleeZombieClass;

	UPROPERTY(EditDefaultsOnly, Category = "Zero")	
	TSubclassOf<AZeroCharacterRangedZombie> RangedZombieClass;

	UPROPERTY(EditDefaultsOnly, Category = "Zero")
	TSubclassOf<AZeroCharacterBossZombie> BossZombieClass;

	UPROPERTY(EditAnywhere, Category = "Start", meta = (AllowPrivateAccess = "true"))
	uint8 StartDay;

};
