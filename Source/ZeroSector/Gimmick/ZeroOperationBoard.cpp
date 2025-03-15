// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroOperationBoard.h"

AZeroOperationBoard::AZeroOperationBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("OperationBoard"));
}

void AZeroOperationBoard::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeroOperationBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

