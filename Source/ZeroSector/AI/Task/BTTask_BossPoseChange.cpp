// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossPoseChange.h"
#include "AIController.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "GameFramework/Character.h"

UBTTask_BossPoseChange::UBTTask_BossPoseChange()
{
	NodeName = TEXT("Change Boss Pose");
}

EBTNodeResult::Type UBTTask_BossPoseChange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	Owner = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr) return EBTNodeResult::Failed;

	ZombieAnim = Cast<UZeroAnimInstanceZombie>(Owner->GetMesh()->GetAnimInstance());
	if (ZombieAnim == nullptr) return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}