// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "AI/Controller/ZeroAIControllerMeleeZombie.h"
#include "AI/Controller/ZeroAIControllerRangedZombie.h"
#include "EngineUtils.h"
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
	EndGame(true);
}

void AZeroGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}
