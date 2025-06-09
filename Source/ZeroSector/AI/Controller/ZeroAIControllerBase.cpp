// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/ZeroAIControllerBase.h"
#include "AI/Controller/ZeroAIControllerBoss.h"
#include "AI/Controller/ZeroAIControllerRangedZombie.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ZeroCharacterPlayer.h"
#include "EngineUtils.h"
#include "ZeroSector.h"

AZeroAIControllerBase::AZeroAIControllerBase()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	// Base Sight 설정
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 150000.f;
	SightConfig->LoseSightRadius = 150000.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AZeroAIControllerBase::OnPerceptionUpdated);

	SetPerceptionComponent(*PerceptionComp);

	TeamID = FGenericTeamId(1);
}

// Hp <= 일 때만 불리기 때문에 무조건 true를 반환
bool AZeroAIControllerBase::IsDead() const
{
	return true;
}

void AZeroAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void AZeroAIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

FGenericTeamId AZeroAIControllerBase::GetGenericTeamId() const
{
	return TeamID;
}

ETeamAttitude::Type AZeroAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(&Other);
	if (OtherTeamAgent)
	{
		FGenericTeamId OtherTeamId = OtherTeamAgent->GetGenericTeamId();
		switch (OtherTeamId)
		{
		case 0:
			return ETeamAttitude::Hostile; // 적군
		case 1:
			return ETeamAttitude::Friendly; // 아군
		case 2:
			return ETeamAttitude::Neutral;
		default:
			return ETeamAttitude::Neutral;
		}
	}
	return ETeamAttitude::Neutral;
}

void AZeroAIControllerBase::RunAI()
{
	UBlackboardComponent* BBComp = Blackboard.Get();
	if (UseBlackboard(BBData, BBComp))
	{
		bool RunResult = RunBehaviorTree(BTData);
	}
}

void AZeroAIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComp)
	{
		BTComp->StopTree();
	}
}

void AZeroAIControllerBase::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*Actor);
	if (Attitude == ETeamAttitude::Hostile)
	{
		Stimulus = CanSenseActor(Actor, EAIPerceptionSense::EPS_Sight);
		HandleSenseSight(Actor, Stimulus);
	}
}

FAIStimulus AZeroAIControllerBase::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
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

void AZeroAIControllerBase::HandleSenseSight(AActor* Actor, const FAIStimulus& AIStimulus)
{
	if (AIStimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
		for (auto AI : TActorRange<AZeroAIControllerBase>(GetWorld()))
		{
			AI->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
		}
		for (auto AI : TActorRange<AZeroAIControllerBoss>(GetWorld()))
		{
			AI->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
		}
		for (auto AI : TActorRange<AZeroAIControllerRangedZombie>(GetWorld()))
		{
			AI->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), Actor);
		}
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), nullptr);
	}
}

