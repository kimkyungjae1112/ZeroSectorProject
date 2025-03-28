// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroUIComponentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UZeroUIComponentInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroUIComponentInterface
{
	GENERATED_BODY()

public:
	virtual class AZeroPlayerController* GetOwnerController() = 0;
	virtual void ChangeInputMode() = 0;
};
