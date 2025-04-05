// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "ZeroCharacterNPC.generated.h"

class UZeroDialogueComponent;

/*
	Speedwagon
	Vaccine
	Criminal
	Normal1
	Normal2
	Normal3
*/

UCLASS()
class ZEROSECTOR_API AZeroCharacterNPC : public AZeroCharacterBase
{
	GENERATED_BODY()

public:
	AZeroCharacterNPC();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	TObjectPtr<UZeroDialogueComponent> DialogueComp;

};
