// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroOperationWidget.h"
#include "Components/Button.h"

UZeroOperationWidget::UZeroOperationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentWeaponType = EWeaponType::ENone;
}

void UZeroOperationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RifleButton = Cast<UButton>(GetWidgetFromName(TEXT("RifleButton")));
	ShotgunButton = Cast<UButton>(GetWidgetFromName(TEXT("ShotgunButton")));
	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("NextButton")));
	CancelButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));

	ensure(RifleButton);
	ensure(ShotgunButton);
	ensure(NextButton);
	ensure(CancelButton);

	RifleButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickRifleButton);
	ShotgunButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickShotgunButton);
	NextButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickNextButton);
	CancelButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickCancelButton);
}

void UZeroOperationWidget::ClickRifleButton()
{
	bIsSeleteced = true;
	CurrentWeaponType = EWeaponType::ERifle;
	RifleButton->SetBackgroundColor(FLinearColor::Green);
	ShotgunButton->SetBackgroundColor(FLinearColor::White);
}

void UZeroOperationWidget::ClickShotgunButton()
{
	bIsSeleteced = true;
	CurrentWeaponType = EWeaponType::EShotgun;
	RifleButton->SetBackgroundColor(FLinearColor::White);
	ShotgunButton->SetBackgroundColor(FLinearColor::Green);
}

void UZeroOperationWidget::ClickNextButton()
{
	if (bIsSeleteced == false) return;
	OnClickNextButton.ExecuteIfBound();
}

void UZeroOperationWidget::ClickCancelButton()
{
	OnCancelButton.ExecuteIfBound();
}
