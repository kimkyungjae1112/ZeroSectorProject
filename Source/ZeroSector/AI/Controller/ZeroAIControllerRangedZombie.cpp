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
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2500.f;
	SightConfig->LoseSightRadius = 2700.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*DamageConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AZeroAIControllerRangedZombie::OnPerceptionUpdated);

	SetPerceptionComponent(*PerceptionComp);

	TeamID = FGenericTeamId(1);
}

void AZeroAIControllerRangedZombie::BeginPlay()
{
	Super::BeginPlay();
}

void AZeroAIControllerRangedZombie::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
	if (Attitude == ETeamAttitude::Hostile)
	{
		Stimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Sight);
		HandleSenseSight(Actor, Stimulus);

		Stimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Damage);
		HandleSenseDamage(Actor, Stimulus);
	}
}

FAIStimulus AZeroAIControllerRangedZombie::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	FAIStimulus ResultStimulus;

	PerceptionComp->GetActorsPerception(Actor, PerceptionInfo);

	TSubclassOf<UAISense> SenseClass =
		(AIPerceptionSense == EAIPerceptionSense::EPS_Sight) ? UAISense_Sight::StaticClass() :
		(AIPerceptionSense == EAIPerceptionSense::EPS_Damage) ? UAISense_Damage::StaticClass() :
		nullptr;

	for (const FAIStimulus& Stimulus : PerceptionInfo.LastSensedStimuli)
	{
		if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == SenseClass)
		{
			ResultStimulus = Stimulus;
			break;
		}
	}
	return ResultStimulus;
}

void AZeroAIControllerRangedZombie::HandleSenseSight(AActor* Actor, const FAIStimulus& Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
	else
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
}

void AZeroAIControllerRangedZombie::HandleSenseDamage(AActor* Actor, const FAIStimulus& Stimulus)
{
}
