// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ZeroStatComponent.h"
#include "ZeroPlayerStatComponent.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroPlayerStatComponent : public UZeroStatComponent
{
	GENERATED_BODY()
	
public:
	UZeroPlayerStatComponent();

protected:
	virtual void BeginPlay() override;
};
