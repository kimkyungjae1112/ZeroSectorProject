// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/ZeroAIControllerMeleeZombie.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "ZeroSector.h"

AZeroAIControllerMeleeZombie::AZeroAIControllerMeleeZombie()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBDataRef(TEXT("/Script/AIModule.BlackboardData'/Game/AI/Test/BB_ZombieTest.BB_ZombieTest'"));
	if (BBDataRef.Object)
	{
		BBData = BBDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTDataRef(TEXT("/Script/AIModule.BehaviorTree'/Game/AI/Test/BT_ZombieTest.BT_ZombieTest'"));
	if (BTDataRef.Object)
	{
		BTData = BTDataRef.Object;
	}

	// Sight 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2200.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
	DamageConfig->SetMaxAge(5.f);

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*DamageConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AZeroAIControllerMeleeZombie::OnPerceptionUpdated);

	SetPerceptionComponent(*PerceptionComp);

	TeamID = FGenericTeamId(1);
}

bool AZeroAIControllerMeleeZombie::IsDead() const
{
	return true;
}

void AZeroAIControllerMeleeZombie::BeginPlay()
{
	Super::BeginPlay();

}

void AZeroAIControllerMeleeZombie::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
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

FAIStimulus AZeroAIControllerMeleeZombie::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	FAIStimulus ResultStimulus;

	PerceptionComp->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

	//AI 가 지금 감지할 수 있는 감각 Sight, Damage
	TSubclassOf<UAISense> QuerySenseClass;
	switch (AIPerceptionSense)
	{
	case EAIPerceptionSense::EPS_None:
		break;
	case EAIPerceptionSense::EPS_Sight:
		QuerySenseClass = UAISense_Sight::StaticClass();
		break;
	case EAIPerceptionSense::EPS_Damage:
		QuerySenseClass = UAISense_Damage::StaticClass();
		break;
	default:
		break;
	}

	TSubclassOf<UAISense> LastSensedStimulusClass;

	for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
	{
		LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);

		if (QuerySenseClass == LastSensedStimulusClass)
		{
			ResultStimulus = AIStimulus;

			return ResultStimulus;
		}
	}

	return ResultStimulus;
}

void AZeroAIControllerMeleeZombie::HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus)
{
	if (AIStimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}

void AZeroAIControllerMeleeZombie::HandleSenseDamage(AActor* Actor, const FAIStimulus& AIStimulus)
{
}
