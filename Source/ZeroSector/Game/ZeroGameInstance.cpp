// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroGameInstance.h"
#include "Component/ZeroDialogueComponent.h"
#include "Character/ZeroCharacterNPC.h"
#include "Game/ZeroGameModeBase.h"
#include "Game/ZeroGameSettingManager.h"
#include "Data/ZeroSingleton.h"
#include "EngineUtils.h"
#include "UI/ZeroEnforceBoardWidget.h"

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
	UZeroEnforceBoardWidget::PistolLevel = 1;
	UZeroEnforceBoardWidget::RifleLevel = 1;
	UZeroEnforceBoardWidget::ShotgunLevel = 1;

	Super::Shutdown();
}

void UZeroGameInstance::Init()
{
	Super::Init();

	SettingManager = NewObject<UZeroGameSettingManager>(this, UZeroGameSettingManager::StaticClass());
	if (SettingManager)
	{
		SettingManager->Init();
	}

	UZeroSingleton::Get().ProvisoDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/Proviso/ProvisoDataTable.ProvisoDataTable"));
}
