// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	void PawnKilled(APawn* PawnKilled);

private:
	void StartWave();
	void EndGame(bool bIsPlayerWinner);

private:
	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TObjectPtr<AZeroZombieSpawner> Spawner;

	UPROPERTY(VisibleAnywhere, Category = "Spawner")
	TSubclassOf<AZeroZombieSpawner> SpawnerClass;
};
