// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Task/BTTask_BossPoseChange.h"
#include "BTTask_BossPose06_09.generated.h"

class UAnimMontage;

UCLASS()
class ZEROSECTOR_API UBTTask_BossPose06_09 : public UBTTask_BossPoseChange
{
	GENERATED_BODY()
	
public:
	UBTTask_BossPose06_09();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void EndMontage(UAnimMontage* Target, bool IsProperlyEnded);

private:
	UPROPERTY(VisibleAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> Pose06_09_Montage;
};
