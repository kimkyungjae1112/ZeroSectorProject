// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ZeroGameModeBase.h"
#include "Player/ZeroPlayerController.h"
#include "AI/Controller/ZeroAIControllerBase.h"
#include "EngineUtils.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "Game/ZeroZombieSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/ZeroDaySequence.h"
#include "Data/ZeroSingleton.h"
#include "Game/ZeroNightPlayerStart.h"
#include "Game/ZeroAfternoonPlayerStart.h"
#include "Gimmick/ZeroWaveTrigger.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroGameSettingManager.h"
#include "Sound/SoundClass.h"
#include "Components/AudioComponent.h"
#include "ZeroSector.h"

uint8 AZeroGameModeBase::Day = 1;

AZeroGameModeBase::AZeroGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Blueprints/Character/B_Player.B_Player_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ZeroSector.ZeroPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<AZeroZombieSpawner> SpawnerClassRef(TEXT("/Game/Blueprints/Game/BP_ZombieSpawner.BP_ZombieSpawner_C"));
	if (SpawnerClassRef.Class)
	{
		SpawnerClass = SpawnerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<AZeroWaveTrigger> WaveTriggerClassRef(TEXT("/Game/Blueprints/Gimmick/BP_WaveTrigger.BP_WaveTrigger_C"));
	if (WaveTriggerClassRef.Class)
	{
		WaveTriggerClass = WaveTriggerClassRef.Class;
	}

	CurrentDaySequence = EDaySequence::EAfternoon;

}

void AZeroGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Spawner = Cast<AZeroZombieSpawner>(UGameplayStatics::GetActorOfClass(this, SpawnerClass));

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC) PC->OnNonClearZmobie.AddUObject(this, &AZeroGameModeBase::RestartLevel);

	UZeroSingleton::Get().ExcludedResearcherName = TEXT("");
	UZeroSingleton::Get().ResetCollectedProvisos();

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;

	USoundBase* AfternoonBGM = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/Sound/RPG_Lite_Edition/Cues/The_Wandering_Hero_Exploration_Theme/CUE_The_Wandering_Hero_Version_02_Fade_Out_Cue.CUE_The_Wandering_Hero_Version_02_Fade_Out_Cue'"));
	if (AfternoonBGM && SM)
	{
		BGMAudioComponent = UGameplayStatics::SpawnSound2D(this, AfternoonBGM);
		if (BGMAudioComponent)
		{
			BGMAudioComponent->SetVolumeMultiplier(SM->GetVolume());
		}
	}
}

void AZeroGameModeBase::InitNight()
{
	SpawnDataTable = UZeroSingleton::Get().GetZombieSpawnData(Day);
	MaxWave = SpawnDataTable.MaxWave;
	CurrentWave = 0;
	ZombieNum = SpawnDataTable.ZombieNum[CurrentWave];
	MaxTime = 500;
	bIsProgress = false;
	ZE_LOG(LogZeroSector, Display, TEXT("MaxWave : %d, ZombieNum : %d"), MaxWave, ZombieNum);
}

void AZeroGameModeBase::ChangeDay()
{
	AZeroDaySequence* DaySequence = Cast<AZeroDaySequence>(UGameplayStatics::GetActorOfClass(GetWorld(), AZeroDaySequence::StaticClass()));
	if (DaySequence)
	{
		if (DaySequence->IsNight())
		{
			DaySequence->NightfallToAfternoon();
			ChangeDayToAfternoon();
		}
		else
		{
			DaySequence->AfternoonToNightfall();
			ChangeDayToNight();
		}
	}
}

void AZeroGameModeBase::PawnKilled(APawn* PawnKilled)
{
	APlayerController* PC = Cast<APlayerController>(PawnKilled->GetController());
	if (PC != nullptr) EndGame(false);

	for (AZeroAIControllerBase* AIController : TActorRange<AZeroAIControllerBase>(GetWorld()))
	{
		if (AIController->IsDead())
		{
			return;
		}
	}

	if (CurrentWave == MaxWave) EndGame(true);
}

void AZeroGameModeBase::StartWave()
{
	if (!bIsProgress) StartTimer();

	// Wave 수 표시
	OnStartNight.ExecuteIfBound(MaxWave - CurrentWave - 1);

	ZombieNum = SpawnDataTable.ZombieNum[CurrentWave];
	CurrentWave++;
	Spawner->SpawnZombie(ZombieNum);
}

void AZeroGameModeBase::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimeTimerHandle, this, &AZeroGameModeBase::DecreaseTime, 1, true);
	bIsProgress = true;
}

void AZeroGameModeBase::RestartLevel()
{
	for(AZeroCharacterBaseZombie* Zombie : TActorRange<AZeroCharacterBaseZombie>(GetWorld()))
	{
		Zombie->Destroy();
	}
	for (AZeroWaveTrigger* Trigger : TActorRange<AZeroWaveTrigger>(GetWorld()))
	{
		Trigger->Destroy();
	}
	ChangeDayToNight();
}

void AZeroGameModeBase::EndGame(bool bIsPlayerWinner)
{
	GetWorld()->GetTimerManager().ClearTimer(TimeTimerHandle);

	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

void AZeroGameModeBase::ChangeDayToAfternoon()
{
	Day++;
	OnStartAfternoon.Broadcast(Day);
	GetWorld()->GetTimerManager().ClearTimer(TimeTimerHandle);
	ZE_LOG(LogZeroSector, Display, TEXT("Day : %d"), Day);

	CurrentDaySequence = EDaySequence::EAfternoon;

	for (AZeroAfternoonPlayerStart* AfternoonPlayerStart: TActorRange<AZeroAfternoonPlayerStart>(GetWorld()))
	{
		if (AfternoonPlayerStart->GetStartDay() == Day)
		{
			// Player를 AfternoonPlayerStart 위치로 이동
			OnAfternoonLocation.Broadcast(AfternoonPlayerStart->GetActorLocation());
		}
	}

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC) PC->ATHUD_Display();
}

void AZeroGameModeBase::ChangeDayToNight()
{
	CurrentDaySequence = EDaySequence::ENight;
	InitNight();

	// 밤 전투맵 위치를 Player 클래스에 전달
	for (AZeroNightPlayerStart* NightPlayerStart : TActorRange<AZeroNightPlayerStart>(GetWorld()))
	{
		if (NightPlayerStart->GetStartDay() == Day)
		{
			// Player를 AfternoonPlayerStart 위치로 이동
			OnNightLocation.ExecuteIfBound(NightPlayerStart->GetActorLocation());

			for (const FVector& Loc : NightPlayerStart->GetZombieSpawnerLoc())
			{
				FRotator Rot(0.0, 0.0, 0.0);
				ZE_LOG(LogZeroSector, Display, TEXT("Spawn Trigger"));
				GetWorld()->SpawnActor<AZeroWaveTrigger>(WaveTriggerClass, Loc, Rot);
			}
		}
	}

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC) PC->NightHUD_Display();

	DecreaseTime();
	OnStartNight.ExecuteIfBound(MaxWave);

	if (BGMAudioComponent)
	{
		BGMAudioComponent->Stop();
		BGMAudioComponent = nullptr;
	}

	// 새로운 밤 브금 재생
	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;

	USoundBase* NightBGM = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Sound/FPS_Menu_Music_Vol_1/Cues/juanjo_-_FPS_Menu_Music_Theme_4_Cue.juanjo_-_FPS_Menu_Music_Theme_4_Cue'"));
	if (NightBGM && SM)
	{
		BGMAudioComponent = UGameplayStatics::SpawnSound2D(this, NightBGM);
		if (BGMAudioComponent)
		{
			BGMAudioComponent->SetVolumeMultiplier(SM->GetVolume());
		}
	}
}

void AZeroGameModeBase::DecreaseTime()
{
	// 남은 시간 표시, 남은 시간이 다 되면 해당 일자는 패배
	MaxTime -= 1;
	OnStartNightForTime.ExecuteIfBound(MaxTime);

	if (MaxTime <= 0) EndGame(false);
}
