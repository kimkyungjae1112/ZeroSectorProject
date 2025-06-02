// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ZeroGameModeBase.h"
#include "Player/ZeroPlayerController.h"
#include "AI/Controller/ZeroAIControllerBase.h"
#include "EngineUtils.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "Character/Zombie/ZeroZombieType.h"
#include "Game/ZeroZombieSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/ZeroDaySequence.h"
#include "Data/ZeroSingleton.h"
#include "Game/ZeroNightPlayerStart.h"
#include "Game/ZeroAfternoonPlayerStart.h"
#include "Gimmick/ZeroWaveTrigger.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroGameSettingManager.h"
#include "Game/ZeroSoundManager.h"
#include "Sound/SoundClass.h"
#include "Components/AudioComponent.h"
#include "UI/ZeroPrologVideoWidget.h"
#include "UI/ZeroEnforceBoardWidget.h"
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
	static ConstructorHelpers::FClassFinder<AZeroWaveTrigger> WaveTriggerClassRef(TEXT("/Game/Blueprints/Gimmick/BP_WaveTrigger.BP_WaveTrigger_C"));
	if (WaveTriggerClassRef.Class)
	{
		WaveTriggerClass = WaveTriggerClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UZeroPrologVideoWidget> PrologWidgetClassRef(TEXT("/Game/Cinematic/Prolog/WBP_PrologWidget.WBP_PrologWidget_C"));
	if (PrologWidgetClassRef.Class)
	{
		PrologWidgetClass = PrologWidgetClassRef.Class;
	}

	CurrentDaySequence = EDaySequence::ENight;

}

void AZeroGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC) PC->OnNonClearZmobie.AddUObject(this, &AZeroGameModeBase::RestartLevel);
	
	PC->SetPrologFlag(PrologFlag);
	if (PrologFlag)
	{
		UZeroPrologVideoWidget* PrologWidget = CreateWidget<UZeroPrologVideoWidget>(GetWorld(), PrologWidgetClass);
		if (PrologWidget)
		{
			PrologWidget->AddToViewport();
		}
	}


	InitNight();

	UZeroSingleton::Get().ExcludedResearcherName = TEXT("");
	UZeroSingleton::Get().ResetCollectedProvisos();

	PlayNightBGM();
}

void AZeroGameModeBase::InitNight()
{
	SpawnDataTable = UZeroSingleton::Get().GetZombieSpawnData(Day);
	MaxWave = SpawnDataTable.MaxWave;
	CurrentWave = 0;
	CommonZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Common].ZombieNum[CurrentWave];
	RangedZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Ranged].ZombieNum[CurrentWave];
	MiniZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Mini].ZombieNum[CurrentWave];
	TankerZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Tanker].ZombieNum[CurrentWave];
	BossZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Boss].ZombieNum[CurrentWave];
	MaxTime = SpawnDataTable.MaxTime;
	bIsProgress = false;
	ZE_LOG(LogZeroSector, Display, TEXT("MaxWave : %d, ZombieNum : %d"), MaxWave, CommonZombieNum);
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
			ZE_LOG(LogZeroSector, Display, TEXT("Night To Afternoon"));
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

	if (CurrentWave == MaxWave)
	{
		if (MaxTime >= SpawnDataTable.MaxTime / 2)
		{
			UZeroEnforceBoardWidget::UpgradePoint += 3;
			ZE_LOG(LogZeroSector, Display, TEXT("Upgrade Point : %d"), UZeroEnforceBoardWidget::UpgradePoint);
		}
		else
		{
			UZeroEnforceBoardWidget::UpgradePoint += 2;
			ZE_LOG(LogZeroSector, Display, TEXT("Upgrade Point : %d"), UZeroEnforceBoardWidget::UpgradePoint);
		}
		EndGame(true);

		UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
		if (GI && GI->GetSoundManager() && GI->GetSoundManager()->WinSFX)
		{
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GI]()
				{
					UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->WinSFX);
				}, 2.0f, false);
		}
	}
}

void AZeroGameModeBase::StartWave()
{
	if (!bIsProgress) StartTimer();

	// Wave 수 표시
	OnStartNight.ExecuteIfBound(MaxWave - CurrentWave - 1);

	CommonZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Common].ZombieNum[CurrentWave];
	RangedZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Ranged].ZombieNum[CurrentWave];
	MiniZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Mini].ZombieNum[CurrentWave];
	TankerZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Tanker].ZombieNum[CurrentWave];
	BossZombieNum = SpawnDataTable.ZombieNums[EZombieType::EZ_Boss].ZombieNum[CurrentWave];
	CurrentWave++;

	for (AZeroZombieSpawner* ZombieSpawner: TActorRange<AZeroZombieSpawner>(GetWorld()))
	{
		if (ZombieSpawner->GetStartDay() == Day)
		{
			ZombieSpawner->SpawnZombie(CommonZombieNum, RangedZombieNum, MiniZombieNum, TankerZombieNum, BossZombieNum);
		}
	}
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

	PlayAfternoonBGM();
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
			OnNightLocation.Broadcast(NightPlayerStart->GetActorLocation());

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

	PlayNightBGM();
}

void AZeroGameModeBase::DecreaseTime()
{
	// 남은 시간 표시, 남은 시간이 다 되면 해당 일자는 패배
	MaxTime -= 1;
	OnStartNightForTime.ExecuteIfBound(MaxTime);

	if (MaxTime <= 0) EndGame(false);

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->LoseSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->LoseSFX);
	}
}

void AZeroGameModeBase::StopBGM()
{
	if (BGMAudioComponent)
	{
		BGMAudioComponent->Stop();
		BGMAudioComponent = nullptr;
	}
}

void AZeroGameModeBase::PlayAfternoonBGM()
{
	StopBGM(); // 기존 BGM 끄기

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;

	USoundBase* AfternoonBGM = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sound/day_bgm.day_bgm'"));
	if (AfternoonBGM && SM)
	{
		BGMAudioComponent = UGameplayStatics::SpawnSound2D(this, AfternoonBGM);
		if (BGMAudioComponent)
		{
			BGMAudioComponent->SetVolumeMultiplier(SM->GetVolume());
		}
	}
}

void AZeroGameModeBase::PlayNightBGM()
{
	StopBGM(); // 기존 BGM 끄기

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;

	USoundBase* NightBGM = LoadObject<USoundBase>(nullptr, TEXT("/Script/Engine.SoundWave'/Game/Sound/night_bgm.night_bgm'"));
	if (NightBGM && SM)
	{
		BGMAudioComponent = UGameplayStatics::SpawnSound2D(this, NightBGM);
		if (BGMAudioComponent)
		{
			BGMAudioComponent->SetVolumeMultiplier(SM->GetVolume());
		}
	}
}
