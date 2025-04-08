// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroLoseScreenWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

UZeroLoseScreenWidget::UZeroLoseScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FadeOutEvent.BindDynamic(this, &UZeroLoseScreenWidget::FadeOutPlay);
	RemoveEvent.BindDynamic(this, &UZeroLoseScreenWidget::RemoveWidget);
}

void UZeroLoseScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Border = Cast<UBorder>(GetWidgetFromName(TEXT("Border")));
}

void UZeroLoseScreenWidget::FadeInPlay()
{
	PlayAnimation(FadeIn);

	BindToAnimationFinished(FadeIn, FadeOutEvent);
}

void UZeroLoseScreenWidget::FadeOutPlay()
{
	PlayAnimation(FadeOut);

	BindToAnimationFinished(FadeOut, RemoveEvent);
}

void UZeroLoseScreenWidget::RemoveWidget()
{
	RemoveFromParent();
}
