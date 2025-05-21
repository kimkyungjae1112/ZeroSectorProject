// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroPrologVideoWidget.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSource.h"

UZeroPrologVideoWidget::UZeroPrologVideoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMediaPlayer> PrologMediaPlayerRef(TEXT("/Script/MediaAssets.MediaPlayer'/Game/Cinematic/ZeroSectorPrologPlayer.ZeroSectorPrologPlayer'"));
	if (PrologMediaPlayerRef.Object)
	{
		PrologMediaPlayer = PrologMediaPlayerRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMediaSource> PrologMediaSourceRef(TEXT("/Script/MediaAssets.FileMediaSource'/Game/Cinematic/ZeroSectorProlog.ZeroSectorProlog'"));
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
}

void UZeroPrologVideoWidget::VideoFinished()
{
	RemoveFromParent();
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		PC->SetPause(false);
		PC->NightHUD_Display();
	}
}
