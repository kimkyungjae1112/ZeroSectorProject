// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Environment/ZeroDayType.h"
#include "Data/ZeroZombieSpawnDataTable.h"
#include "ZeroGameModeBase.generated.h"

class AZeroZombieSpawner;

DECLARE_DELEGATE_OneParam(FOnStartNight, uint8)
DECLARE_DELEGATE_OneParam(FOnStartNightForTime, int32)

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

	FOnStartNight OnStartNight;
	FOnStartNightForTime OnStartNightForTime;

	static uint8 Day;

	void InitDay();
	void ChangeDay();
	void PawnKilled(APawn* PawnKilled);

private:
	void StartWave();
	void EndGame(bool bIsPlayerWinner);
	void DecreaseTime();

private:
	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TObjectPtr<AZeroZombieSpawner> Spawner;

	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TSubclassOf<AZeroZombieSpawner> SpawnerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Env")
	EDaySequence CurrentDaySequence;

	UPROPERTY()
	FZeroZombieSpawnDataTable SpawnDataTable;

	uint8 MaxWave;
	uint8 CurrentWave;
	uint8 ZombieNum;

	FTimerHandle TimeTimerHandle;
	int32 MaxTime;
};
