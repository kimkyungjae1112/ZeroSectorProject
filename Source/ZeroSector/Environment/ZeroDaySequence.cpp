// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/ZeroDaySequence.h"
#include "ZeroSector.h"

AZeroDaySequence::AZeroDaySequence(const FObjectInitializer& Init) : Super(Init)
{
}

void AZeroDaySequence::BeginPlay()
{
	Super::BeginPlay();

	Pause();
	SetTimeOfDay(12.f);
}

void AZeroDaySequence::AfternoonToNightfall()
{
	SetTimeOfDay(0.f);
	ZE_LOG(LogZeroSector, Display, TEXT("낮 -> 밤"));
}

void AZeroDaySequence::NightfallToAfternoon()
{
	SetTimeOfDay(12.f);
	ZE_LOG(LogZeroSector, Display, TEXT("밤 -> 낮"));
}

bool AZeroDaySequence::IsNight()
{
	return 0.f + GetTimeOfDay() < 0.1f;
}
