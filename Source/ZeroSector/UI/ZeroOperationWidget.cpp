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
	UTexture2D* ShotgunTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/shotgun_noselect.shotgun_noselect'"));
	UTexture2D* RifleSelectedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/rifle_select.rifle_select'"));

	bIsSeleteced = true;
	CurrentWeaponType = EWeaponType::ERifle;

	FSlateBrush Brush;

	Brush.SetResourceObject(ShotgunTexture);
	Brush.ImageSize = FVector2D(400, 125);
	ShotgunButton->WidgetStyle.Normal = Brush;

	Brush.SetResourceObject(RifleSelectedTexture);
	Brush.ImageSize = FVector2D(400, 125);
	RifleButton->WidgetStyle.Normal = Brush;

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroOperationWidget::ClickShotgunButton()
{

	UTexture2D* ShotgunSelectedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/shotgun_select.shotgun_select'"));
	UTexture2D* RifleTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/rifle_noselect.rifle_noselect'"));

	bIsSeleteced = true;
	CurrentWeaponType = EWeaponType::EShotgun;

	FSlateBrush Brush;

	Brush.SetResourceObject(ShotgunSelectedTexture);
	Brush.ImageSize = FVector2D(400, 125);
	ShotgunButton->WidgetStyle.Normal = Brush;

	Brush.SetResourceObject(RifleTexture);
	Brush.ImageSize = FVector2D(400, 125);
	RifleButton->WidgetStyle.Normal = Brush;

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
