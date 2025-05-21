// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroOperationWidget.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

UZeroOperationWidget::UZeroOperationWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CurrentWeaponType = EWeaponType::ENone;
}

void UZeroOperationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UZeroGameInstance>(GetGameInstance());

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

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroOperationWidget::ClickShotgunButton()
{
	bIsSeleteced = true;
	CurrentWeaponType = EWeaponType::EShotgun;
	RifleButton->SetBackgroundColor(FLinearColor::White);
	ShotgunButton->SetBackgroundColor(FLinearColor::Green);

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroOperationWidget::ClickNextButton()
{
	if (bIsSeleteced == false) return;
	OnClickNextButton.ExecuteIfBound();

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroOperationWidget::ClickCancelButton()
{
	OnCancelButton.ExecuteIfBound();

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}
