// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/SunMoonDaySequenceActor.h"
#include "ZeroDaySequence.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroDaySequence : public ASunMoonDaySequenceActor
{
	GENERATED_BODY()
	
public:
	AZeroDaySequence(const FObjectInitializer& Init);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	void DayToNightfall();
};
