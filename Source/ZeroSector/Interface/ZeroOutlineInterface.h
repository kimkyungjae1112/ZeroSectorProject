// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroOutlineInterface.generated.h"

UINTERFACE(MinimalAPI)
class UZeroOutlineInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroOutlineInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlayMaterial() = 0;
	virtual void SetUnOverlayMaterial() = 0;
};
