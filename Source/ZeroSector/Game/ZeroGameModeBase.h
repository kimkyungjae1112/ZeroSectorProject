// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Environment/ZeroDayType.h"
#include "Data/ZeroZombieSpawnDataTable.h"
#include "ZeroGameModeBase.generated.h"

class AZeroZombieSpawner;
class AZeroWaveTrigger;

DECLARE_DELEGATE_OneParam(FOnStartNight, uint8)
DECLARE_DELEGATE_OneParam(FOnStartNightForTime, int32)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartAfternoon, uint8)
DECLARE_DELEGATE_OneParam(FOnNightLocation, const FVector&)
DECLARE_DELEGATE_OneParam(FOnAfternoonLocation, const FVector&)

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

	// 밤이 됐을 때 이동할 위치
	FOnNightLocation OnNightLocation;

	// 낮이 됐을 때 이동할 위치
	FOnAfternoonLocation OnAfternoonLocation;

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

	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TSubclassOf<AZeroWaveTrigger> WaveTriggerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Env")
	EDaySequence CurrentDaySequence;

	UPROPERTY()
	FZeroZombieSpawnDataTable SpawnDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Setting")
	uint8 MaxWave;

	UPROPERTY(VisibleAnywhere, Category = "Setting")
	uint8 CurrentWave;

	UPROPERTY(VisibleAnywhere, Category = "Setting")
	uint8 ZombieNum;

	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 MaxTime;
	
	FTimerHandle TimeTimerHandle;
	bool bIsProgress = false;
};
