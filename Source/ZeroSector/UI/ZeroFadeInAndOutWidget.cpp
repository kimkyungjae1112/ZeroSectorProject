// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroFadeInAndOutWidget.h"
#include "Components/Border.h"
#include "Animation/WidgetAnimation.h"
#include "ZeroSector.h"

UZeroFadeInAndOutWidget::UZeroFadeInAndOutWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FadeOutEvent.BindDynamic(this, &UZeroFadeInAndOutWidget::FadeOutPlay);
	RemoveEvent.BindDynamic(this, &UZeroFadeInAndOutWidget::RemoveWidget);
}

void UZeroFadeInAndOutWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Border = Cast<UBorder>(GetWidgetFromName(TEXT("Border")));
	ensure(Border);
}

void UZeroFadeInAndOutWidget::FadeInPlay()
{
	PlayAnimation(FadeIn);
	
	BindToAnimationFinished(FadeIn, FadeOutEvent);
}

void UZeroFadeInAndOutWidget::FadeOutPlay()
{
	PlayAnimation(FadeOut);

	BindToAnimationFinished(FadeOut, RemoveEvent);
}

void UZeroFadeInAndOutWidget::RemoveWidget()
{
	RemoveFromParent();
}
