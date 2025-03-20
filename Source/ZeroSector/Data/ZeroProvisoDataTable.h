// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroProvisoDataTable.generated.h"

USTRUCT(BlueprintType)
struct FZeroProvisoDataTable : public FTableRowBase
{
    GENERATED_BODY()

    public:
    FZeroProvisoDataTable()
        : ProvisoName(NAME_None), Description(TEXT("")), ResearcherInfo(TEXT(""))
    {
    }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proviso")
    FName ProvisoName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proviso")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proviso")
    FString ResearcherInfo;

    bool operator==(const FZeroProvisoDataTable& Other) const
    {
        return ProvisoName == Other.ProvisoName;
    }
};
