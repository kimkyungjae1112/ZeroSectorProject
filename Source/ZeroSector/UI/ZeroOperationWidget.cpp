// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroOperationWidget.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/ZeroSingleton.h"
#include "Game/ZeroGameModeBase.h"
#include "ZeroSector.h"

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
	Image = Cast<UImage>(GetWidgetFromName(TEXT("BackGround")));
	CommonZombieNumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("CommonZombieText")));
	TankerZombieNumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TankerZombieText")));
	MiniZombieNumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MiniZombieText")));
	RangedZombieNumText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RangedZombieText")));

	ensure(RifleButton);
	ensure(ShotgunButton);
	ensure(NextButton);
	ensure(CancelButton);
	ensure(Image);
	ensure(CommonZombieNumText);
	ensure(TankerZombieNumText);
	ensure(MiniZombieNumText);
	ensure(RangedZombieNumText);

	RifleButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickRifleButton);
	ShotgunButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickShotgunButton);
	NextButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickNextButton);
	CancelButton->OnClicked.AddDynamic(this, &UZeroOperationWidget::ClickCancelButton);

	SpawnDataTable = UZeroSingleton::Get().GetZombieSpawnData(AZeroGameModeBase::Day);

	uint8 CommonZombieNum{ 0 };
	uint8 RangedZombieNum{ 0 };
	uint8 MiniZombieNum{ 0 };
	uint8 TankerZombieNum{ 0 };

	for (auto E : SpawnDataTable.ZombieNums[EZombieType::EZ_Common].ZombieNum)
		CommonZombieNum += E;
	
	for (auto E : SpawnDataTable.ZombieNums[EZombieType::EZ_Tanker].ZombieNum)
		TankerZombieNum += E;

	for (auto E : SpawnDataTable.ZombieNums[EZombieType::EZ_Mini].ZombieNum)
		MiniZombieNum += E;

	for (auto E : SpawnDataTable.ZombieNums[EZombieType::EZ_Ranged].ZombieNum)
		RangedZombieNum += E;
	
	CommonZombieNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), CommonZombieNum)));
	TankerZombieNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), TankerZombieNum)));
	MiniZombieNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), MiniZombieNum)));
	RangedZombieNumText->SetText(FText::FromString(FString::Printf(TEXT("%d"), RangedZombieNum)));
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

void UZeroOperationWidget::BackgroundImageChange(uint8 Day)
{
	ZE_LOG(LogZeroSector, Display, TEXT("Image Change 함수 실행"));

	switch (Day)
	{
	case 2:
	{
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/OpBoard_Day2.OpBoard_Day2'"));
		Image->SetBrushFromTexture(NewTexture, true);
		break;
	}
	case 3:
	{
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/OpBoard_Day3.OpBoard_Day3'"));
		Image->SetBrushFromTexture(NewTexture, true);
		break;
	}
	case 4:
	{
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/OpBoard_Day4.OpBoard_Day4'"));
		Image->SetBrushFromTexture(NewTexture, true);
		break;
	}
	case 5:
	{
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/OpBoard_Day5.OpBoard_Day5'"));
		Image->SetBrushFromTexture(NewTexture, true);
		break;
	}
	case 6:
	{
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/OpBoard_Day6.OpBoard_Day6'"));
		Image->SetBrushFromTexture(NewTexture, true);
		break;
	}
	case 7:
	{
		UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/OpBoard_Day7.OpBoard_Day7'"));
		Image->SetBrushFromTexture(NewTexture, true);
		break;
	}
	default:
	{
		ZE_LOG(LogZeroSector, Display, TEXT("이미지 안들어옴"));
		break;
	}
	}
}
