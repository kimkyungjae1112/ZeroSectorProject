// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroStartSetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZeroStartSetInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroStartSetInterface
{
	GENERATED_BODY()

public:
	virtual void StartSet() = 0;
};
