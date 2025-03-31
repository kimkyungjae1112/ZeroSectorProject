// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "AI/Controller/ZeroAIControllerMeleeZombie.h"
#include "AI/Controller/ZeroAIControllerRangedZombie.h"
#include "EngineUtils.h"
#include "Game/ZeroZombieSpawner.h"
#include "Kismet/GameplayStatics.h"
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
}

void AZeroGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Spawner = Cast<AZeroZombieSpawner>(UGameplayStatics::GetActorOfClass(this, SpawnerClass));
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
	
	StartWave();
	
	//EndGame(true);
}

void AZeroGameModeBase::StartWave()
{
	// if 마지막 웨이브라면
	//		if 좀비를 다 잡았다면
	//			EndGame(true)
	Spawner->SpawnZombie();
}

void AZeroGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
