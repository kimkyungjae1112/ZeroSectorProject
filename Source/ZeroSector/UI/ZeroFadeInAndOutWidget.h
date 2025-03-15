// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroFadeInAndOutWidget.generated.h"

class UWidgetAnimation;
class UBorder;

UCLASS()
class ZEROSECTOR_API UZeroFadeInAndOutWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UZeroFadeInAndOutWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	void virtual NativeConstruct() override;

public:
	void FadeInPlay();

private:
	UFUNCTION()
	void FadeOutPlay();

private:
	FWidgetAnimationDynamicEvent FadeOutEvent;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBorder> Border;
};
