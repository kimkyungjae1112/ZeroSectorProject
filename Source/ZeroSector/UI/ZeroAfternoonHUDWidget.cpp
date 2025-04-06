// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroAfternoonHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ZeroHUDInterface.h"

UZeroAfternoonHUDWidget::UZeroAfternoonHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxActivePoint = -1.f;
}

void UZeroAfternoonHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ActivePointBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ActivePoint")));

	ensure(ActivePointBar);

	IZeroHUDInterface* Interface = Cast<IZeroHUDInterface>(GetOwningPlayerPawn());
	if (Interface)	Interface->SetAfternoonHUDWidget(this);
}

void UZeroAfternoonHUDWidget::SetMaxActivePoint(float InMaxActivePoint)
{
	MaxActivePoint = InMaxActivePoint;
}

void UZeroAfternoonHUDWidget::UpdateAPBar(float InActivePoint)
{
	ActivePointBar->SetPercent(InActivePoint / MaxActivePoint);
}
