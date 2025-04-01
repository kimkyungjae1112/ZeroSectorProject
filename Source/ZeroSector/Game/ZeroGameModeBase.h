// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Environment/ZeroDayType.h"
#include "ZeroGameModeBase.generated.h"

class AZeroZombieSpawner;

UCLASS()
class ZEROSECTOR_API AZeroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZeroGameModeBase();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE EDaySequence GetDaySequence() const { return CurrentDaySequence; }

	void PawnKilled(APawn* PawnKilled);
	void ChangeDay();

private:
	void StartWave();
	void EndGame(bool bIsPlayerWinner);

private:
	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TObjectPtr<AZeroZombieSpawner> Spawner;

	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TSubclassOf<AZeroZombieSpawner> SpawnerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Env")
	EDaySequence CurrentDaySequence;

	uint8 Day;
	uint8 MaxWave;
	uint8 CurrentWave;

};
