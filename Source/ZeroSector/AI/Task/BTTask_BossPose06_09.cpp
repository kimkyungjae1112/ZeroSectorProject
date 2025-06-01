// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossPose06_09.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"

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

	if (!ZombieAnim->bIsPose06) return EBTNodeResult::Succeeded;

	ZombieAnim->Montage_Play(Pose06_09_Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UBTTask_BossPose06_09::EndMontage);
	ZombieAnim->Montage_SetEndDelegate(MontageEnd, Pose06_09_Montage);

	float MontageLength = Pose06_09_Montage->GetPlayLength();
	float CurrentPosition = ZombieAnim->Montage_GetPosition(Pose06_09_Montage);
	float RemainingTime = MontageLength - CurrentPosition;
	RemainingTime = FMath::Max(RemainingTime, 0.01f);

	FTimerHandle TimerHandle;
	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			UBTTaskNode::FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}, RemainingTime, false);


	return EBTNodeResult::InProgress;
}

void UBTTask_BossPose06_09::EndMontage(UAnimMontage* Target, bool IsProperlyEnded)
{
	ZombieAnim->bIsPose06 = false;
}