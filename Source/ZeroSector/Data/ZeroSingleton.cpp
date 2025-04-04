// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ZeroSingleton.h"

UZeroSingleton::UZeroSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ProvisoDataTableRef(TEXT("/Game/Data/Proviso/ProvisoDataTable.ProvisoDataTable"));
	if (ProvisoDataTableRef.Succeeded())
	{
		const UDataTable* ProvisoDataTable = ProvisoDataTableRef.Object;

		TArray<uint8*> ValueArray;
		ProvisoDataTable->GetRowMap().GenerateValueArray(ValueArray);

		for (uint8* Value : ValueArray)
		{
			if (Value)
			{
				FZeroProvisoDataTable* Data = reinterpret_cast<FZeroProvisoDataTable*>(Value);
			}
		}

		Algo::Transform(ValueArray, ProvisoDataList,
			[](uint8* Value)
			{
				return *reinterpret_cast<FZeroProvisoDataTable*>(Value);
			}
		);
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
