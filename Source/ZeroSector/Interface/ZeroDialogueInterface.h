// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroDialogueInterface.generated.h"

DECLARE_DELEGATE(FOnFinishedDialogue)

UINTERFACE(MinimalAPI)
class UZeroDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroDialogueInterface
{
	GENERATED_BODY()

public:
	virtual void StartDialogue() = 0;
	virtual void SetupFinishedDialogueDelegate(const FOnFinishedDialogue& InOnFinishedDialogue) = 0;
};
