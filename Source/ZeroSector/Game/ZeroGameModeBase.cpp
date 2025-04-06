// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ZeroGameModeBase.h"
#include "Player/ZeroPlayerController.h"
#include "AI/Controller/ZeroAIControllerMeleeZombie.h"
#include "AI/Controller/ZeroAIControllerRangedZombie.h"
#include "EngineUtils.h"
#include "Game/ZeroZombieSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Environment/ZeroDaySequence.h"
#include "Data/ZeroSingleton.h"
#include "ZeroSector.h"

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

	CurrentDaySequence = EDaySequence::EAfternoon;
	Day = 1;
}

void AZeroGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Spawner = Cast<AZeroZombieSpawner>(UGameplayStatics::GetActorOfClass(this, SpawnerClass));

}

void AZeroGameModeBase::InitDay()
{
	SpawnDataTable = UZeroSingleton::Get().GetZombieSpawnData(Day);
	MaxWave = SpawnDataTable.MaxWave;
	CurrentWave = 0;
	ZombieNum = SpawnDataTable.ZombieNum[CurrentWave];
	Day++;
	MaxTime = 300;
	ZE_LOG(LogZeroSector, Display, TEXT("MaxWave : %d, ZombieNum : %d"), MaxWave, ZombieNum);
}

void AZeroGameModeBase::ChangeDay()
{
	AZeroDaySequence* DaySequence = Cast<AZeroDaySequence>(UGameplayStatics::GetActorOfClass(GetWorld(), AZeroDaySequence::StaticClass()));
	if (DaySequence)
	{
		if (DaySequence->IsNight())
		{
			GetWorld()->GetTimerManager().ClearTimer(TimeTimerHandle);

			CurrentDaySequence = EDaySequence::EAfternoon;
			DaySequence->NightfallToAfternoon();
			
			AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
			if (PC)
			{
				PC->ATHUD_Display();
			}
		}
		else
		{
			CurrentDaySequence = EDaySequence::ENight;
			DaySequence->AfternoonToNightfall();
			InitDay();

			AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
			if (PC)
			{
				PC->NightHUD_Display();
			}
			StartWave();
			GetWorld()->GetTimerManager().SetTimer(TimeTimerHandle, this, &AZeroGameModeBase::DecreaseTime, 1, true);
		}
	}
}

void AZeroGameModeBase::PawnKilled(APawn* PawnKilled)
{
	APlayerController* PC = Cast<APlayerController>(PawnKilled->GetController());
	if (PC != nullptr)
	{
		EndGame(false);
	}

	for (AZeroAIControllerMeleeZombie* AIController : TActorRange<AZeroAIControllerMeleeZombie>(GetWorld()))
	{
		if (AIController->IsDead())
		{
			return;
		}
	}
	for (AZeroAIControllerRangedZombie* AIController : TActorRange<AZeroAIControllerRangedZombie>(GetWorld()))
	{
		if (AIController->IsDead())
		{
			return;
		}
	}
	
	// 이제 트리거에서 GameMode를 가져온 다음 StartWave를 불러주면 됨.
	FTimerHandle TempTimer;
	GetWorld()->GetTimerManager().SetTimer(TempTimer, [&]()
		{
			StartWave();
		}, 3.f, false);
}

void AZeroGameModeBase::StartWave()
{
	if (CurrentWave == MaxWave)
	{
		EndGame(true);
		return;
	}
	// Wave 수 표시
	OnStartNight.ExecuteIfBound(MaxWave - CurrentWave - 1);

	ZombieNum = SpawnDataTable.ZombieNum[CurrentWave];
	CurrentWave++;
	Spawner->SpawnZombie(ZombieNum);

	ZE_LOG(LogZeroSector, Display, TEXT("%d"), CurrentWave);
}

void AZeroGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}

void AZeroGameModeBase::DecreaseTime()
{
	// 남은 시간 표시, 남은 시간이 다 되면 해당 일자는 패배
	MaxTime -= 1;
	OnStartNightForTime.ExecuteIfBound(MaxTime);

	if (MaxTime <= 0) EndGame(false);
}
