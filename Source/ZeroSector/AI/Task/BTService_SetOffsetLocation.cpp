// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTService_SetOffsetLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Actor.h"

UBTService_SetOffsetLocation::UBTService_SetOffsetLocation()
{
	NodeName = TEXT("Set Target Offset Location");
	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UBTService_SetOffsetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject("Target"));


	if (!AIPawn || !Target) return;

	FVector ToAI = AIPawn->GetActorLocation() - Target->GetActorLocation();
	FVector Dir = ToAI.GetSafeNormal();

	FVector OffsetPos = Target->GetActorLocation() + Dir * DesiredDistance;

	BB->SetValueAsVector("TargetOffsetLocation", OffsetPos);


}