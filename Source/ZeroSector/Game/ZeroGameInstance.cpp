// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroGameInstance.h"
#include "Component/ZeroDialogueComponent.h"
#include "Character/ZeroCharacterNPC.h"
#include "Game/ZeroGameModeBase.h"
#include "EngineUtils.h"

UZeroGameInstance::UZeroGameInstance()
{
}

void UZeroGameInstance::Shutdown()
{
	for (AZeroCharacterNPC* NPC : TActorRange<AZeroCharacterNPC>(GetWorld()))
	{
		NPC->GetComponentByClass<UZeroDialogueComponent>()->ShutdownGame();
	}

	AZeroGameModeBase::Day = 1;

	Super::Shutdown();
}
