// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroHUDWidget.h"
#include "Components/TextBlock.h"
#include "Interface/ZeroHUDInterface.h"
#include "Game/ZeroGameModeBase.h"
#include "Components/ProgressBar.h"
#include "ZeroSector.h"

UZeroHUDWidget::UZeroHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.f;
}

void UZeroHUDWidget::UpdateCurrentAmmo(int32 InCurrentAmmo)
{
	CurrentAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InCurrentAmmo)));
}

void UZeroHUDWidget::UpdateMaxAmmo(int32 InMaxAmmo)
{
	MaxAmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InMaxAmmo)));
}

void UZeroHUDWidget::UpdateRemainTime(int32 InTime)
{
	int32 Minute = InTime / 60;
	int32 Second = InTime % 60;
	RemainMinuteText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Minute)));
	RemainSecondText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Second)));
}

void UZeroHUDWidget::UpdateRemainWave(uint8 InWave)
{
	RemainWaveText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InWave)));
}

void UZeroHUDWidget::SetMaxHp(float InMaxHp)
{
	MaxHp = InMaxHp;
}

void UZeroHUDWidget::UpdateHpBar(float InHp)
{
	HpBar->SetPercent(InHp / MaxHp);
	ZE_LOG(LogZeroSector, Display, TEXT("체력 : %f"), InHp / MaxHp);
}

void UZeroHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurrentAmmo")));
	MaxAmmoText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxAmmo")));
	RemainMinuteText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RemainMinute")));
	RemainSecondText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RemainSecond")));
	RemainWaveText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RemainWave")));
	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HpBar")));

	ensure(CurrentAmmoText);
	ensure(MaxAmmoText);
	ensure(RemainMinuteText);
	ensure(RemainSecondText);
	ensure(RemainWaveText);
	ensure(HpBar);

	IZeroHUDInterface* Interface = Cast<IZeroHUDInterface>(GetOwningPlayerPawn());
	if (Interface)	Interface->SetHUDWidget(this);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnStartNight.BindUObject(this, &UZeroHUDWidget::UpdateRemainWave);
		GameMode->OnStartNightForTime.BindUObject(this, &UZeroHUDWidget::UpdateRemainTime);
	}
}
