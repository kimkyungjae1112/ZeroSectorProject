// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ZeroZombieMeleeAttackNotifyState.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroZombieMeleeAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UZeroZombieMeleeAttackNotifyState();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	void MakeSphereTrace(class ACharacter* Character);
	bool CanComboAttack(AActor* Owner);

private:
	UPROPERTY()
	TSet<AActor*> HitTarget;
};
