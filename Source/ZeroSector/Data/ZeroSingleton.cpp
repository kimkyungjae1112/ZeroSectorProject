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
