// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "ZeroProvisoFakeActor.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroProvisoFakeActor : public AZeroProvisoActor
{
	GENERATED_BODY()
	
public:
	AZeroProvisoFakeActor();

protected:
	virtual void BeginPlay() override;
	
};
