// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/ZeroAIControllerBase.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"

AZeroAIControllerBase::AZeroAIControllerBase()
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

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
}

void AZeroAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
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
		if (OtherTeamId == TeamID)
		{
			return ETeamAttitude::Friendly; // 아군
		}
		else if (OtherTeamId != TeamID)
		{
			return ETeamAttitude::Hostile; // 적군
		}
		else
		{
			return ETeamAttitude::Neutral;
		}
	}
	return ETeamAttitude::Neutral; // 중립
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

