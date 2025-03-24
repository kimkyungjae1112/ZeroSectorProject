// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroHUDInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZeroHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroHUDInterface
{
	GENERATED_BODY()

public:
	virtual void SetHUDWidget(class UZeroHUDWidget* InHUDWidget) = 0;
};
