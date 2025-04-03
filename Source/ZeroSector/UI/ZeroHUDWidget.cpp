// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroHUDWidget.h"
#include "Components/TextBlock.h"
#include "Interface/ZeroHUDInterface.h"
#include "ZeroSector.h"

UZeroHUDWidget::UZeroHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroHUDWidget::UpdateCurrentAmmo(uint8 InCurrentAmmo)
{
	CurrentAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InCurrentAmmo)));
}

void UZeroHUDWidget::UpdateMaxAmmo(uint8 InMaxAmmo)
{
	MaxAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InMaxAmmo)));
}

void UZeroHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentAmmo")));
	MaxAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxAmmo")));
	
	ensure(CurrentAmmoText);
	ensure(MaxAmmoText);

	IZeroHUDInterface* Interface = Cast<IZeroHUDInterface>(GetOwningPlayerPawn());
	if (Interface)	Interface->SetHUDWidget(this);
}
