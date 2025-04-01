// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ZeroDialogueDataTable.h"
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

	FORCEINLINE FZeroDialogueDataTable GetDialogueTable(int32 RowIndex) const { return DialogueTables.IsValidIndex(RowIndex) ? DialogueTables[RowIndex] : FZeroDialogueDataTable(); }
	FORCEINLINE FZeroProvisoDataTable GetProvisoData(int32 RowIndex) const { return ProvisoDataList.IsValidIndex(RowIndex) ? ProvisoDataList[RowIndex] : FZeroProvisoDataTable(); }
	FORCEINLINE FZeroZombieSpawnDataTable GetZombieSpawnData(uint8 RowIndex) const 
	{
		FName Row = *FString::FromInt(RowIndex);
		return SpawnDataTable[Row];
	}

	/* 단서 데이터 */
	TArray<FZeroProvisoDataTable> GetCollectedProvisos() const;
	void AddCollectedProviso(const FZeroProvisoDataTable& ProvisoData);

private:
	UPROPERTY()
	TArray<FZeroDialogueDataTable> DialogueTables;

	UPROPERTY()
	TArray<FZeroProvisoDataTable> ProvisoDataList;

	UPROPERTY()
	TArray<FZeroProvisoDataTable> CollectedProvisos;

	UPROPERTY()
	TMap<FName, FZeroZombieSpawnDataTable> SpawnDataTable;
};
