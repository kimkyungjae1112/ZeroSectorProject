// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroOperationWidget.h"
#include "Components/Button.h"

UZeroOperationWidget::UZeroOperationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroOperationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RifleButton = Cast<UButton>(GetWidgetFromName(TEXT("RifleButton")));
	ShotgunButton = Cast<UButton>(GetWidgetFromName(TEXT("ShotgunButton")));
	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("NextButton")));

	ensure(RifleButton);
	ensure(ShotgunButton);
	ensure(NextButton);

	RifleButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickRifleButton);
	ShotgunButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickShotgunButton);
	NextButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickNextButton);
}

void UZeroOperationWidget::ClickRifleButton()
{
	CurrentWeaponType = EWeaponType::EZeroRifle;
	RifleButton->SetBackgroundColor(FLinearColor::Green);
	ShotgunButton->SetBackgroundColor(FLinearColor::White);
}

void UZeroOperationWidget::ClickShotgunButton()
{
	CurrentWeaponType = EWeaponType::EZeroShotgun;
	RifleButton->SetBackgroundColor(FLinearColor::White);
	ShotgunButton->SetBackgroundColor(FLinearColor::Green);
}

void UZeroOperationWidget::ClickNextButton()
{
	OnClickNextButton.ExecuteIfBound();
}
