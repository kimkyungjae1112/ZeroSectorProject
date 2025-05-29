// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Controller/ZeroAIControllerBase.h"
#include "ZeroAIControllerBoss.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroAIControllerBoss : public AZeroAIControllerBase
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;
};
