// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZeroGameModeBase.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AZeroGameModeBase();

public:
	void PawnKilled(APawn* PawnKilled);

private:
	void EndGame(bool bIsPlayerWinner);
};
