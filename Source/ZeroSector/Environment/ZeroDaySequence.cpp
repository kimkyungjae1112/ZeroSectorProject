// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/ZeroDaySequence.h"

AZeroDaySequence::AZeroDaySequence(const FObjectInitializer& Init) : Super(Init)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZeroDaySequence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AZeroDaySequence::BeginPlay()
{
	Super::BeginPlay();

	Pause();
}

void AZeroDaySequence::DayToNightfall()
{
	SetTimeOfDay(0.f);
}
