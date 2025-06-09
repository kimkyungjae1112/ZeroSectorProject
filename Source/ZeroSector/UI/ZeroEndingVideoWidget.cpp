// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroEndingVideoWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "UI/ZeroEndingWidget.h"
#include "Game/ZeroGameModeBase.h"

UZeroEndingVideoWidget::UZeroEndingVideoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> EndingMediaPlayerRef(TEXT("/Script/MediaAssets.MediaPlayer'/Game/Cinematic/Ending/ZeroSectorEndingPlayer.ZeroSectorEndingPlayer'"));
	if (EndingMediaPlayerRef.Object)
	{
		EndingMediaPlayer = EndingMediaPlayerRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMediaSource> EndingMediaSourceRef(TEXT("/Script/MediaAssets.FileMediaSource'/Game/Cinematic/Ending/ZeroSectorEnding.ZeroSectorEnding'"));
	if (EndingMediaSourceRef.Object)
	{
		EndingMediaSource = EndingMediaSourceRef.Object;
	}
	static ConstructorHelpers::FClassFinder<UZeroEndingWidget> EndingWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_EndingWidget.WBP_EndingWidget_C"));
	if (EndingWidgetClassRef.Class)
	{
		EndingWidgetClass = EndingWidgetClassRef.Class;
	}
}

void UZeroEndingVideoWidget::HappyEndingDisplay() const
{
	UZeroEndingWidget* EndingWidget = CreateWidget<UZeroEndingWidget>(GetWorld(), EndingWidgetClass);
	if (EndingWidget)
	{
		EndingWidget->AddToViewport();
		EndingWidget->FadeOutPlay();
		EndingWidget->HappyEndingDisplay();
	}
}

void UZeroEndingVideoWidget::NormalEndingDisplay() const
{
	UZeroEndingWidget* EndingWidget = CreateWidget<UZeroEndingWidget>(GetWorld(), EndingWidgetClass);
	if (EndingWidget)
	{
		EndingWidget->AddToViewport();
		EndingWidget->FadeOutPlay();
		EndingWidget->NormalEndingDisplay();
	}
}

void UZeroEndingVideoWidget::BadEndingDisplay() const
{
	UZeroEndingWidget* EndingWidget = CreateWidget<UZeroEndingWidget>(GetWorld(), EndingWidgetClass);
	if (EndingWidget)
	{
		EndingWidget->AddToViewport();
		EndingWidget->FadeOutPlay();
		EndingWidget->BadEndingDisplay();
	}
}

void UZeroEndingVideoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetPause(true);
	}

	EndingMediaPlayer->OpenSource(EndingMediaSource);
	EndingMediaPlayer->Rewind();
	EndingMediaPlayer->Play();
	EndingMediaPlayer->OnEndReached.AddDynamic(this, &UZeroEndingVideoWidget::VideoFinished);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayCinematicSound(3);
	}
}

void UZeroEndingVideoWidget::VideoFinished()
{
	RemoveFromParent();

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->AllHUD_Close();
	}

	switch (EndingIndex)
	{
	case 1:
		HappyEndingDisplay();
		break;
	case 2:
		NormalEndingDisplay();
		break;
	case 3:
		BadEndingDisplay();
		break;
	default:
		break;
	}

	
}
