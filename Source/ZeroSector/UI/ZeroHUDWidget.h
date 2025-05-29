// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroHUDWidget.generated.h"

class UZeroCrossHairWidget;
class UTextBlock;
class UProgressBar;
class UImage;

UCLASS()
class ZEROSECTOR_API UZeroHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroHUDWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateCurrentAmmo(int32 InCurrentAmmo);
	void UpdateMaxAmmo(int32 InMaxAmmo);
	// void UpdateCurrentWeapon(int8 InCurrentWeapon);
	void UpdateRemainTime(int32 InTime);
	void UpdateRemainWave(uint8 InWave);

	void SetMaxHp(float InMaxHp);
	void UpdateHpBar(float InHp);

	float GetCurrentSpread() const;
	void IncreaseSpread(float Amount) const;
	void HitCrosshair() const;
	void UnHitCrosshair() const;


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

	/*UPROPERTY(VisibleAnywhere)
	TObjectPtr<UImage> CurrentWeaponImage;*/

	float MaxHp;
};
