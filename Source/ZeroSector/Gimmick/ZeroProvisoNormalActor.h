// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "ZeroProvisoNormalActor.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroProvisoNormalActor : public AZeroProvisoActor
{
	GENERATED_BODY()

public:
	AZeroProvisoNormalActor();

protected:
	virtual void BeginPlay() override;
};
