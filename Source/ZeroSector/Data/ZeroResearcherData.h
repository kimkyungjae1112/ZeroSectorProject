// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ZeroResearcherData.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroResearcherData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Age;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Trust;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Portrait;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ProvisoStart;  

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 ProvisoNum;       
	
};
