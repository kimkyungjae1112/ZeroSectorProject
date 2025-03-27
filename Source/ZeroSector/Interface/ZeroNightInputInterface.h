// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroNightInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZeroNightInputInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroNightInputInterface
{
	GENERATED_BODY()

public:
	virtual class USpringArmComponent* GetSpringArmComp() = 0;
	virtual void NightToAfternoon() = 0;
};
