// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroAfternoonInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZeroAfternoonInputInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroAfternoonInputInterface
{
	GENERATED_BODY()

public:
	virtual void DisplayInteractUI() = 0;
	virtual void CloseInteractUI() = 0;

	virtual void StartDialogueCameraView() = 0;
	virtual void EndDialogueCameraView() = 0;
};
