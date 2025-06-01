// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossRushAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ZeroCharacterAIInterface.h"

UBTTask_BossRushAttack::UBTTask_BossRushAttack()
{
	NodeName = TEXT("Rush Attack");
}

EBTNodeResult::Type UBTTask_BossRushAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IZeroCharacterAIInterface* AIPawn = Cast<IZeroCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	FOnAttackTwoFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackTwoDelegate(OnAttackFinished);
	AIPawn->AttackTwoByAI();
	return EBTNodeResult::InProgress;
}
