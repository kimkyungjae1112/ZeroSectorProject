// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ZeroSingleton.h"

UZeroSingleton::UZeroSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ProvisoDataTableRef(TEXT("/Game/Data/Proviso/ProvisoDataTable.ProvisoDataTable"));
	if (ProvisoDataTableRef.Succeeded())
	{
		const UDataTable* Table = ProvisoDataTableRef.Object;
		for (const auto& Elem : Table->GetRowMap())
		{
			const FName& RowName = Elem.Key;
			const FZeroProvisoDataTable* Data = reinterpret_cast<const FZeroProvisoDataTable*>(Elem.Value);
			if (Data)
			{
				ProvisoDataMap.Add(RowName, *Data);
			}
		}
	}

	static ConstructorHelpers::FObjectFinder<UDataTable> ZombieSpawnDataRef(TEXT("/Script/Engine.DataTable'/Game/Data/Zombie/ZeroZombieSpawnDataTable.ZeroZombieSpawnDataTable'"));
	if (ZombieSpawnDataRef.Succeeded())
	{
		const UDataTable* SpawnDataTableBuf = ZombieSpawnDataRef.Object;
		check(SpawnDataTableBuf->GetRowMap().Num() > 0);

		for (const auto& Elem : SpawnDataTableBuf->GetRowMap())
		{
			const FName& RowName = Elem.Key;
			const FZeroZombieSpawnDataTable* StatPtr = reinterpret_cast<const FZeroZombieSpawnDataTable*>(Elem.Value);
			if (StatPtr)
			{
				SpawnDataTable.Add(RowName, *StatPtr);
			}
		}
	}

}

void UZeroSingleton::AddCollectedProviso(const FZeroProvisoDataTable& ProvisoData)
{
	if (!CollectedProvisos.Contains(ProvisoData))
	{
		CollectedProvisos.Add(ProvisoData);
	}
}


FZeroProvisoDataTable UZeroSingleton::GetRandomProvisoByType(EZeroProvisoType Type)
{
	if (!ProvisoDataTable) return FZeroProvisoDataTable();

	TArray<FZeroProvisoDataTable*> AllRows;
	ProvisoDataTable->GetAllRows(TEXT("GetAllProvisos"), AllRows);

	TArray<FZeroProvisoDataTable*> Matched;

	for (FZeroProvisoDataTable* Row : AllRows)
	{
		if (Row && Row->ProvisoType == Type && !Row->ProvisoName.IsNone())
		{
			bool bAlreadyCollected = CollectedProvisos.ContainsByPredicate(
				[&](const FZeroProvisoDataTable& Collected)
				{
					return Collected.ProvisoName == Row->ProvisoName;
				}
			);

			bool bIsExcludedResearcher = !ExcludedResearcherName.IsEmpty() &&
				Row->ResearcherInfo.Equals(ExcludedResearcherName, ESearchCase::IgnoreCase);

			if (!bAlreadyCollected && !bIsExcludedResearcher)
			{
				Matched.Add(Row);
			}
		}
	}

	if (Matched.Num() == 0) return FZeroProvisoDataTable();

	int32 Index = FMath::RandRange(0, Matched.Num() - 1);
	return *Matched[Index];
}


void UZeroSingleton::ResetCollectedProvisos()
{
	CollectedProvisos.Empty();
}


TArray<FZeroProvisoDataTable> UZeroSingleton::GetCollectedProvisos() const
{
	return CollectedProvisos;
}

UZeroSingleton& UZeroSingleton::Get()
{
	UZeroSingleton* Singleton = CastChecked<UZeroSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	return *NewObject<UZeroSingleton>();
}

