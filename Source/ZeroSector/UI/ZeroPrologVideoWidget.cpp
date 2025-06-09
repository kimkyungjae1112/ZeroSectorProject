// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroPrologVideoWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Game/ZeroGameModeBase.h"
#include "Interface/ZeroStartSetInterface.h"

UZeroPrologVideoWidget::UZeroPrologVideoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> PrologMediaPlayerRef(TEXT("/Script/MediaAssets.MediaPlayer'/Game/Cinematic/Prolog/ZeroSectorProlog_Player.ZeroSectorProlog_Player'"));
	if (PrologMediaPlayerRef.Object)
	{
		PrologMediaPlayer = PrologMediaPlayerRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMediaSource> PrologMediaSourceRef(TEXT("/Script/MediaAssets.FileMediaSource'/Game/Cinematic/Prolog/ZeroSectorProlog.ZeroSectorProlog'"));
	if (PrologMediaSourceRef.Object)
	{
		PrologMediaSource = PrologMediaSourceRef.Object;
	}
}

void UZeroPrologVideoWidget::NativeConstruct()
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
	PrologMediaPlayer->OnEndReached.AddDynamic(this, &UZeroPrologVideoWidget::VideoFinished);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayCinematicSound(1);
	}
}

void UZeroPrologVideoWidget::VideoFinished()
{
	RemoveFromParent();
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetPause(false);
		PC->NightHUD_Display();

		IZeroStartSetInterface* SSI = Cast<IZeroStartSetInterface>(PC->GetPawn());
		if (SSI) SSI->StartSet();
	}

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PlayNightBGM();
	}
}
