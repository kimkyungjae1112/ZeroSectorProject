// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroHUDWidget.generated.h"

class UZeroCrossHairWidget;
class UTextBlock;
class UProgressBar;

UCLASS()
class ZEROSECTOR_API UZeroHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroHUDWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateCurrentAmmo(int32 InCurrentAmmo);
	void UpdateMaxAmmo(int32 InMaxAmmo);
	void UpdateRemainTime(int32 InTime);
	void UpdateRemainWave(uint8 InWave);

	void SetMaxHp(float InMaxHp);
	void UpdateHpBar(float InHp);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UZeroCrossHairWidget> CrosshairWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> CurrentAmmoText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> MaxAmmoText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> RemainMinuteText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> RemainSecondText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> RemainWaveText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProgressBar> HpBar;

	float MaxHp;
};
