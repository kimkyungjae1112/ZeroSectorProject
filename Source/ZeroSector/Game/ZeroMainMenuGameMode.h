// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZeroMainMenuGameMode.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    AZeroMainMenuGameMode();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UZeroMainMenuWidget> MainMenuWidgetClass;
};
