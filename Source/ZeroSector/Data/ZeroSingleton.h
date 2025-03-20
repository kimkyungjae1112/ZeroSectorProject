// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/ZeroDialogueDataTable.h"
#include "Data/ZeroProvisoDataTable.h"
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

	TArray<FZeroProvisoDataTable> GetCollectedProvisos() const;

	void AddCollectedProviso(const FZeroProvisoDataTable& ProvisoData);

private:
	TArray<FZeroDialogueDataTable> DialogueTables;

	UPROPERTY()
	TArray<FZeroProvisoDataTable> ProvisoDataList;

	UPROPERTY()
	TArray<FZeroProvisoDataTable> CollectedProvisos;
};
