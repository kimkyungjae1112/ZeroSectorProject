// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroAfternoonPlayerStart.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroAfternoonPlayerStart : public AActor
{
	GENERATED_BODY()
	
public:
	AZeroAfternoonPlayerStart();

public:
	FORCEINLINE uint8 GetStartDay() const { return StartDay; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditAnywhere, Category = "Start", meta = (AllowPrivateAccess = "true"))
	uint8 StartDay;
};
