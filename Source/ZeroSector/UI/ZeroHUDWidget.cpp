// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroHUDWidget.h"
#include "UI/ZeroCrossHairWidget.h"
#include "Components/TextBlock.h"
#include "Interface/ZeroHUDInterface.h"
#include "Game/ZeroGameModeBase.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "ZeroSector.h"

UZeroHUDWidget::UZeroHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PistolImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/handgun.handgun'"));
	RifleImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/rifle.rifle'"));
	ShotgunImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/shotgun.shotgun'"));

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

void UZeroHUDWidget::UpdateCurrentWeapon(const EWeaponType& WeaponType)
{
	switch (WeaponType)
	{
	case EWeaponType::EPistol:
		CurrentWeaponImage->SetBrushFromTexture(PistolImage);
		break;
	case EWeaponType::ERifle:
		CurrentWeaponImage->SetBrushFromTexture(RifleImage);
		break;
	case EWeaponType::EShotgun:
		CurrentWeaponImage->SetBrushFromTexture(ShotgunImage);
		break;
	default:
		ZE_LOG(LogZeroSector, Display, TEXT("무기 이미지 안들어옴"));
	}
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

float UZeroHUDWidget::GetCurrentSpread() const
{
	return CrosshairWidget->GetCurrentSpread();
}

void UZeroHUDWidget::IncreaseSpread(float Amount) const
{
	CrosshairWidget->IncreaseSpread(Amount);
}

void UZeroHUDWidget::HitCrosshair() const
{
	CrosshairWidget->HitCrosshair();
}

void UZeroHUDWidget::UnHitCrosshair() const
{
	CrosshairWidget->UnHitCrosshair();
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
	CrosshairWidget = Cast<UZeroCrossHairWidget>(GetWidgetFromName(TEXT("WBP_CrossHair")));
	CurrentWeaponImage = Cast<UImage>(GetWidgetFromName(TEXT("CurrentWeapon")));

	ensure(CurrentAmmoText);
	ensure(MaxAmmoText);
	ensure(RemainMinuteText);
	ensure(RemainSecondText);
	ensure(RemainWaveText);
	ensure(HpBar);
	ensure(CurrentWeaponImage);
	CurrentWeaponImage->SetBrushFromTexture(PistolImage);

	IZeroHUDInterface* Interface = Cast<IZeroHUDInterface>(GetOwningPlayerPawn());
	if (Interface)	Interface->SetHUDWidget(this);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnStartNight.BindUObject(this, &UZeroHUDWidget::UpdateRemainWave);
		GameMode->OnStartNightForTime.BindUObject(this, &UZeroHUDWidget::UpdateRemainTime);
	}
}
