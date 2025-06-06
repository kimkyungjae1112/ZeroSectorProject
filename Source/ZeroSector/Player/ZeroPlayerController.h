// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeroPlayerController.generated.h"

class UZeroHUDWidget;
class UZeroAfternoonHUDWidget;
class UZeroLoseScreenWidget;
class UZeroNoteWidget;
class UZeroEndingVideoWidget;

DECLARE_MULTICAST_DELEGATE(FOnClearZombie)
DECLARE_MULTICAST_DELEGATE(FOnNonClearZombie)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStartInterview, FString)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEndInterview, FString)

UCLASS()
class ZEROSECTOR_API AZeroPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AZeroPlayerController();

	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	FORCEINLINE void SetPrologFlag(bool InPrologFlag) { PrologFlag = InPrologFlag; }
	FORCEINLINE UZeroAfternoonHUDWidget* GetAfternoonHUDWidget() const { return AfternoonHUDWidgetPtr; }

public:
	FOnClearZombie OnClearZombie;
	FOnNonClearZombie OnNonClearZmobie;

	FOnStartInterview OnStartInterview;
	FOnEndInterview OnEndInterview;

	void InputModeGameOnly();
	void InputModeUIOnly();
	void InputModeGameAndUI();

	void ApplyBlurEffect();
	void RemoveBlurEffect();

	void ATHUD_Display();
	void NightHUD_Display();
	void AllHUD_Close() const;

	// 크로스 헤어
	float GetCurrentSpread() const;
	void CrosshairSpread(float Amount) const;
	void HitCrosshair() const;
	void UnHitCrosshair() const;

	void StartInterview() const;
	void EndInterview() const;

	// 데이터를 public 으로 꺼내놓고 개발을 진행하니까 이제 바꿀 수가 없음;
	// private으로 넣으라니까 캡슐화좀 ㅜㅜㅜ
	UPROPERTY()
	FString SelectedInterviewName;

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

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UUserWidget> BlurWidgetInstance;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> BlurWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UUserWidget> WinScreenPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WinScreenClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroLoseScreenWidget> LoseScreenPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroLoseScreenWidget> LoseScreenClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroEndingVideoWidget> EndingCinematicClass;

private:
	bool PrologFlag{ false };
};
