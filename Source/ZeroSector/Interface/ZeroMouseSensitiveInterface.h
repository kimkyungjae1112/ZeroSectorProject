// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroMouseSensitiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class UZeroMouseSensitiveInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroMouseSensitiveInterface
{
	GENERATED_BODY()

public:
	virtual float GetMouseSensitive() const = 0;
	virtual	void SetMouseSensitive(float InMouseSensitive) = 0;
};
