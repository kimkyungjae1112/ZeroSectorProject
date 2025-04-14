// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroEnforceBoardWidget.generated.h"

class UTextBlock;
class UButton;

DECLARE_DELEGATE(FOnCancelButton)

UCLASS()
class ZEROSECTOR_API UZeroEnforceBoardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UZeroEnforceBoardWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	void NativeConstruct() override;

public:
	FOnCancelButton OnCancelButton;

private:
	UFUNCTION()
	void PistolEnfoButtonClicked();

	UFUNCTION()
	void RifleEnfoButtonClicked();

	UFUNCTION()
	void ShotgunEnfoButtonClicked();

	UFUNCTION()
	void CloseEnfoButtonClicked();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> PistolLevelText;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> RifleLevelText;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> ShotgunLevelText;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> PistolEnfoButton;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> RifleEnfoButton;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> ShotgunEnfoButton;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> CancelEnfoButton;
};
