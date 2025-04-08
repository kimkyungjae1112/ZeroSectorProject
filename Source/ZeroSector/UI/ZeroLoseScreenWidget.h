// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroLoseScreenWidget.generated.h"

class UBorder;
class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroLoseScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroLoseScreenWidget(const FObjectInitializer& ObjectInitializer);

protected:
	void virtual NativeConstruct() override;

public:
	void FadeInPlay();

private:
	UFUNCTION()
	void FadeOutPlay();

	UFUNCTION()
	void RemoveWidget();

private:
	FWidgetAnimationDynamicEvent FadeOutEvent;
	FWidgetAnimationDynamicEvent RemoveEvent;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBorder> Border;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> LoseText;

};
