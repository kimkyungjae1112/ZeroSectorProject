// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroWallVideoWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Game/ZeroGameModeBase.h"

UZeroWallVideoWidget::UZeroWallVideoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> PrologMediaPlayerRef(TEXT("/Script/MediaAssets.MediaPlayer'/Game/Cinematic/Wall/ZeroSectorWall_Player.ZeroSectorWall_Player'"));
	if (PrologMediaPlayerRef.Object)
	{
		PrologMediaPlayer = PrologMediaPlayerRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMediaSource> PrologMediaSourceRef(TEXT("/Script/MediaAssets.FileMediaSource'/Game/Cinematic/Wall/ZeroSectorWall.ZeroSectorWall'"));
	if (PrologMediaSourceRef.Object)
	{
		PrologMediaSource = PrologMediaSourceRef.Object;
	}
}

void UZeroWallVideoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetPause(true);
	}

	PrologMediaPlayer->OpenSource(PrologMediaSource);
	PrologMediaPlayer->Rewind();
	PrologMediaPlayer->Play();
	PrologMediaPlayer->OnEndReached.AddDynamic(this, &UZeroWallVideoWidget::VideoFinished);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayCinematicSound(2);
	}
}

void UZeroWallVideoWidget::VideoFinished()
{
	RemoveFromParent();
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetPause(false);
		PC->NightHUD_Display();
	}

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayNightBGM();
	}
}