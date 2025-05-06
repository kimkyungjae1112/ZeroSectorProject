// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Controller/ZeroAIControllerNPC.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Game/ZeroGameModeBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "ZeroSector.h"

AZeroAIControllerNPC::AZeroAIControllerNPC()
{
	
}

void AZeroAIControllerNPC::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		ZE_LOG(LogZeroSector, Display, TEXT("탐지 성공"));
		MoveToNextPoint();
	}
	else if (Result.Code.GetValue() == EPathFollowingRequestResult::Failed)
	{
		ZE_LOG(LogZeroSector, Display, TEXT("탐지 실패"));
	}
}

void AZeroAIControllerNPC::MoveToNextPoint()
{
	if (WayPoints[AZeroGameModeBase::Day].WayPoint.Num() == 0) return;

	MoveToLocation(WayPoints[AZeroGameModeBase::Day].WayPoint[CurrentIndex]);
	CurrentIndex = (CurrentIndex + 1) % WayPoints[AZeroGameModeBase::Day].WayPoint.Num();
}


void AZeroAIControllerNPC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//RunAI();
}

void AZeroAIControllerNPC::BeginPlay()
{
	Super::BeginPlay();

	MoveToNextPoint();
}


