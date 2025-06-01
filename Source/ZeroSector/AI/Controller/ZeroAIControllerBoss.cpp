// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/ZeroAIControllerBoss.h"
#include "BehaviorTree/BlackboardComponent.h"

void AZeroAIControllerBoss::BeginPlay()
{
	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsBool(TEXT("bIsMove"), true);
	GetBlackboardComponent()->SetValueAsBool(TEXT("bIsSpawnAttack"), false);
}
