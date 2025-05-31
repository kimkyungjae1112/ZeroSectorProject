// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossPose06_09.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"

UBTTask_BossPose06_09::UBTTask_BossPose06_09()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Pose06_09_MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Zombie/AM/AM_BossPose06_09.AM_BossPose06_09'"));
	if (Pose06_09_MontageRef.Object)
	{
		Pose06_09_Montage = Pose06_09_MontageRef.Object;
	}

	NodeName = TEXT("Pose 06 To 09");
}

EBTNodeResult::Type UBTTask_BossPose06_09::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	ZombieAnim->bIsPose06 = false;
	return EBTNodeResult::Succeeded;
}