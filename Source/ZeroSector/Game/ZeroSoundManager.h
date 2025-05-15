// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZeroSoundManager.generated.h"

class USoundBase;

UCLASS()
class ZEROSECTOR_API UZeroSoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<USoundBase> PauseSFX;

	UPROPERTY()
	TObjectPtr<USoundBase> ProvisoInteractSFX; 

	UPROPERTY()
	TObjectPtr<USoundBase> UIClickSFX;

	void Init();
};
