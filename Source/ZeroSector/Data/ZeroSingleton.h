// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Data/ZeroCharacterStat.h"
#include "Data/ZeroZombieSpawnDataTable.h"
#include "ZeroSingleton.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroSingleton : public UObject
{
	GENERATED_BODY()
	
public:
    UZeroSingleton();
    static UZeroSingleton& Get();

    FORCEINLINE FZeroZombieSpawnDataTable GetZombieSpawnData(uint8 RowIndex) const
    {
        FName Row = *FString::FromInt(RowIndex);
        return SpawnDataTable[Row];
    }
        
    FORCEINLINE FZeroProvisoDataTable GetProvisoData(FName RowName) const
    {
        if (const FZeroProvisoDataTable* Found = ProvisoDataMap.Find(RowName))
        {
            return *Found;
        }
        return FZeroProvisoDataTable();
    }


	/* 단서 데이터 */
	TArray<FZeroProvisoDataTable> GetCollectedProvisos() const;
	void AddCollectedProviso(const FZeroProvisoDataTable& ProvisoData);

    void ResetCollectedProvisos();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data", meta = (AllowPrivateAccess = true))
    UDataTable* ProvisoDataTable;

    FZeroProvisoDataTable GetRandomProvisoByType(EZeroProvisoType Type);

    UPROPERTY()
    FString ExcludedResearcherName;

    UPROPERTY()
    FString SelectedResearcherName;


private:
    UPROPERTY()
    TMap<FName, FZeroProvisoDataTable> ProvisoDataMap;

	UPROPERTY()
	TArray<FZeroProvisoDataTable> CollectedProvisos;

	UPROPERTY()
	TMap<FName, FZeroZombieSpawnDataTable> SpawnDataTable;

    
};
