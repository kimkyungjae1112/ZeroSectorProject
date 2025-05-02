// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/ZeroAIControllerRangedZombie.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

AZeroAIControllerRangedZombie::AZeroAIControllerRangedZombie()
{
	SightConfig->SightRadius = 2500.f;
	SightConfig->LoseSightRadius = 2700.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	TeamID = FGenericTeamId(1);
}

bool AZeroAIControllerRangedZombie::IsDead() const
{
	return true;
}

void AZeroAIControllerRangedZombie::BeginPlay()
{
	Super::BeginPlay();
}
