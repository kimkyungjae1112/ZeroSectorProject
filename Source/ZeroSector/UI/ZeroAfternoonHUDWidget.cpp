// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroAfternoonHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ZeroHUDInterface.h"

UZeroAfternoonHUDWidget::UZeroAfternoonHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroAfternoonHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ActivePoint = Cast<UProgressBar>(GetWidgetFromName(TEXT("ActivePoint")));

	ensure(ActivePoint);

	IZeroHUDInterface* Interface = Cast<IZeroHUDInterface>(GetOwningPlayerPawn());
	if (Interface)	Interface->SetAfternoonHUDWidget(this);
}
