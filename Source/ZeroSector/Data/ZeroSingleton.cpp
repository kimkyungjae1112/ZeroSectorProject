// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ZeroSingleton.h"

UZeroSingleton::UZeroSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DialogueDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable.DialogueDataTable'"));
	if (DialogueDataTableRef.Succeeded())
	{
		const UDataTable* DialogueDataTable = DialogueDataTableRef.Object;
		//check(DialogueDataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DialogueDataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, DialogueTables,
			[](uint8* Value)
			{
				return *reinterpret_cast<FZeroDialogueDataTable*>(Value);
			}
		);
	}

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
