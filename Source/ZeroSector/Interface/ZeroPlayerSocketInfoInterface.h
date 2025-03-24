// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZeroPlayerSocketInfoInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UZeroPlayerSocketInfoInterface : public UInterface
{
	GENERATED_BODY()
};

class ZEROSECTOR_API IZeroPlayerSocketInfoInterface
{
	GENERATED_BODY()

public:
	virtual FTransform GetWeaponSocketTransform(const FName& SocketName) const = 0;
};
