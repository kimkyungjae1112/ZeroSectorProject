// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroEndingWidget.generated.h"

class UImage;
class UTexture2D;
class UWidgetAnimation;

UCLASS()
class ZEROSECTOR_API UZeroEndingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UZeroEndingWidget(const FObjectInitializer& ObjectInitializer);

	void HappyEndingDisplay() const;
	void NormalEndingDisplay() const;
	void BadEndingDisplay() const;

protected:
	void NativeConstruct() override;

public:
	void FadeOutPlay();

private:
	UFUNCTION()
	void FadeInPlay();

	UFUNCTION()
	void RemoveWidget();

private:
	FWidgetAnimationDynamicEvent FadeInEvent;
	FWidgetAnimationDynamicEvent RemoveEvent;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> FadeOut;

	UPROPERTY(VisibleAnywhere, Category = "Image")
	TObjectPtr<UTexture2D> HappyEndingImage;

	UPROPERTY(VisibleAnywhere, Category = "Image")
	TObjectPtr<UTexture2D> NormalEndingImage;

	UPROPERTY(VisibleAnywhere, Category = "Image")
	TObjectPtr<UTexture2D> BadEndingImage;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	TObjectPtr<UImage> EndingImage;
};
