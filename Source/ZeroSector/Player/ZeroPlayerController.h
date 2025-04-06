// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroPlayerController.generated.h"

class UZeroHUDWidget;
class UZeroAfternoonHUDWidget;

DECLARE_DELEGATE(FOnClearZombie)

UCLASS()
class ZEROSECTOR_API AZeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZeroPlayerController();

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

public:
	FOnClearZombie OnClearZombie;

	void InputModeGameOnly();
	void InputModeUIOnly();
	void InputModeGameAndUI();

	void ApplyBlurEffect();
	void RemoveBlurEffect();

	void ATHUD_Display();
	void NightHUD_Display();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroHUDWidget> HUDWidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroAfternoonHUDWidget> AfternoonHUDWidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroAfternoonHUDWidget> AfternoonHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> BlurWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BlurWidgetClass;
};
