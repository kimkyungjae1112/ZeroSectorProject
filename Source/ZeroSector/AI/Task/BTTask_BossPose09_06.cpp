// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BossPose09_06.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"

UBTTask_BossPose09_06::UBTTask_BossPose09_06()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Pose09_06_MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Animation/Zombie/AM/AM_BossPose09_06.AM_BossPose09_06'"));
	if (Pose09_06_MontageRef.Object)
	{
		Pose09_06_Montage = Pose09_06_MontageRef.Object;
	}

	NodeName = TEXT("Pose 09 To 06");
}

EBTNodeResult::Type UBTTask_BossPose09_06::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type SuperResult = Super::ExecuteTask(OwnerComp, NodeMemory);

	ZombieAnim->Montage_Play(Pose09_06_Montage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UBTTask_BossPose09_06::EndMontage);
	ZombieAnim->Montage_SetEndDelegate(MontageEnd, Pose09_06_Montage);

	float MontageLength = Pose09_06_Montage->GetPlayLength();
	float CurrentPosition = ZombieAnim->Montage_GetPosition(Pose09_06_Montage);
	float RemainingTime = MontageLength - CurrentPosition;
	RemainingTime = FMath::Max(RemainingTime, 0.01f);

	FTimerHandle TimerHandle;
	Owner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			UBTTaskNode::FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}, RemainingTime, false);


	return EBTNodeResult::InProgress;
}

void UBTTask_BossPose09_06::EndMontage(UAnimMontage* Target, bool IsProperlyEnded)
{
	ZombieAnim->bIsPose06 = true;
}
