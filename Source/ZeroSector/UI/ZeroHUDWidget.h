// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroHUDWidget.generated.h"

class UZeroCrossHairWidget;
class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroHUDWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateCurrentAmmo(uint8 InCurrentAmmo);
	void UpdateMaxAmmo(uint8 InMaxAmmo);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UZeroCrossHairWidget> CrosshairWidget;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> CurrentAmmoText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> MaxAmmoText;

};
