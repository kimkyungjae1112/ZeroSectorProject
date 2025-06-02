// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroEndingVideoWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

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
}

void UZeroEndingVideoWidget::VideoFinished()
{
	RemoveFromParent();
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuMap"));
	}
}
