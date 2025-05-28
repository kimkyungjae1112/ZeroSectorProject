// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroEnforceBoardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Component/Input/ZeroInputNightComponent.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Game/ZeroGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ZeroSector.h"

int32 UZeroEnforceBoardWidget::PistolLevel = 1;
int32 UZeroEnforceBoardWidget::RifleLevel = 1;
int32 UZeroEnforceBoardWidget::ShotgunLevel = 1;
int32 UZeroEnforceBoardWidget::UpgradePoint = 0;

UZeroEnforceBoardWidget::UZeroEnforceBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroEnforceBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UZeroGameInstance>(GetGameInstance());

	PistolLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PistolLevel")));
	RifleLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RifleLevel")));
	ShotgunLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShotgunLevel")));
	PointText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PointText")));

	PistolEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("PistolEnfoButton")));
	RifleEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("RifleEnfoButton")));
	ShotgunEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("ShotgunEnfoButton")));
	CancelEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));

	ensure(PistolLevelText);
	ensure(RifleLevelText);
	ensure(ShotgunLevelText);
	ensure(PointText);

	ensure(PistolEnfoButton);
	ensure(RifleEnfoButton);
	ensure(ShotgunEnfoButton);
	ensure(CancelEnfoButton);

	PistolEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::PistolEnfoButtonClicked);
	RifleEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::RifleEnfoButtonClicked);
	ShotgunEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::ShotgunEnfoButtonClicked);
	CancelEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::CloseEnfoButtonClicked);

	SetPistolLevelText(PistolLevel);
	SetRifleLevelText(RifleLevel);
	SetShotgunLevelText(ShotgunLevel);
	SetPointText();
}

void UZeroEnforceBoardWidget::PistolEnfoButtonClicked()
{
	if (PistolLevel == 7 || UpgradePoint <= 0) return;
	SetPistolLevelText(++PistolLevel);

	UpgradePoint--;
	SetPointText();
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroEnforceBoardWidget::RifleEnfoButtonClicked()
{
	if (RifleLevel == 7 || UpgradePoint <= 0) return;
	SetRifleLevelText(++RifleLevel);

	UpgradePoint--;
	SetPointText();
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroEnforceBoardWidget::ShotgunEnfoButtonClicked()
{
	if (ShotgunLevel == 7 || UpgradePoint <= 0) return;
	SetShotgunLevelText(++ShotgunLevel);

	UpgradePoint--;
	SetPointText();
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroEnforceBoardWidget::CloseEnfoButtonClicked()
{
	OnCancelButton.ExecuteIfBound();

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroEnforceBoardWidget::SetPistolLevelText(int32 InLevel)
{
	PistolLevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InLevel)));
}

void UZeroEnforceBoardWidget::SetRifleLevelText(int32 InLevel)
{
	RifleLevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InLevel)));
}

void UZeroEnforceBoardWidget::SetShotgunLevelText(int32 InLevel)
{
	ShotgunLevelText->SetText(FText::FromString(FString::Printf(TEXT("%d"), InLevel)));
}

void UZeroEnforceBoardWidget::SetPointText()
{
	PointText->SetText(FText::FromString(FString::Printf(TEXT("%d"), UpgradePoint)));

	TArray<UButton*> Buttons = { PistolEnfoButton, RifleEnfoButton, ShotgunEnfoButton };

	if (UpgradePoint <= 0)
	{
		for (int32 i = 0; i < 3; ++i)
		{
			if (Buttons.IsValidIndex(i) && Buttons[i])
			{
				Buttons[i]->SetIsEnabled(false);
			}
		}
	}
	else
	{
		for (int32 i = 0; i < 3; ++i)
		{
			if (Buttons.IsValidIndex(i) && Buttons[i])
			{
				Buttons[i]->SetIsEnabled(true);
			}
		}
	}
}
