// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_Death.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"

UBTTask_Death::UBTTask_Death()
{
	NodeName = TEXT("Death");
}

EBTNodeResult::Type UBTTask_Death::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (AICon && AICon->BrainComponent)
	{
		AICon->BrainComponent->StopLogic(TEXT("Death"));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

