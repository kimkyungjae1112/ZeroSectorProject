// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroPlayerController.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZeroPlayerController();

public:
	void InputModeGameOnly();
	void InputModeGameAndUI();

protected:
	virtual void BeginPlay() override;
};
