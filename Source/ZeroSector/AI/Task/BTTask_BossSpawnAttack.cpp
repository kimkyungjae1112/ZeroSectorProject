// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossSpawnAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ZeroCharacterAIInterface.h"

UBTTask_BossSpawnAttack::UBTTask_BossSpawnAttack()
{
	NodeName = TEXT("Boss Spawn Attack");
}

EBTNodeResult::Type UBTTask_BossSpawnAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (Owner == nullptr) return EBTNodeResult::Failed;

	IZeroCharacterAIInterface* AIPawn = Cast<IZeroCharacterAIInterface>(Owner);
	if (nullptr == AIPawn) return EBTNodeResult::Failed;

	FOnSpawnAttackFinished OnSpawnAttackFinished;
	OnSpawnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAISpawnAttackDelegate(OnSpawnAttackFinished);
	AIPawn->SpawnAttackByAI();

	return EBTNodeResult::InProgress;
}
