// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroPlayerController.generated.h"

class UZeroHUDWidget;

UCLASS()
class ZEROSECTOR_API AZeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZeroPlayerController();

public:
	void InputModeGameOnly();
	void InputModeUIOnly();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroHUDWidget> HUDWidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroHUDWidget> HUDWidgetClass;
};
