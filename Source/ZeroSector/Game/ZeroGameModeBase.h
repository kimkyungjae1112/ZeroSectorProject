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
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartAfternoon, uint8)

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

	// 좀비 스폰을 위한 데이터 넘겨주는 delegate
	FOnStartNight OnStartNight;
	
	// 시간 표기를 위한 delegate
	FOnStartNightForTime OnStartNightForTime;

	// 낮으로 넘어갈 때 데이터 초기화를 위한 delegate
	// DialogueComp에서 사용
	FOnStartAfternoon OnStartAfternoon;

	static uint8 Day;

	void InitNight();
	void ChangeDay();
	void PawnKilled(APawn* PawnKilled);

	void StartWave();
	void StartTimer();
	void RestartLevel();

private:
	void EndGame(bool bIsPlayerWinner);
	
	void ChangeDayToAfternoon();
	void ChangeDayToNight();
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
	bool bIsProgress = false;
};
