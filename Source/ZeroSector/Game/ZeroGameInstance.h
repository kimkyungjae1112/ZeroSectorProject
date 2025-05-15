// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ZeroGameInstance.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UZeroGameInstance();

	virtual void Shutdown() override;
	virtual void Init() override;
	
	UPROPERTY()
	class UZeroGameSettingManager* SettingManager;

	UPROPERTY()
	class UZeroSoundManager* SoundManager;

	UZeroSoundManager* GetSoundManager() const { return SoundManager; }
};
