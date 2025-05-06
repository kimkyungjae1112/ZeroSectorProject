// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "ZeroCharacterNPC.generated.h"

class UZeroDialogueComponent;
class UZeroNPCWidgetComponent;
class UZeroNPCNameWidget;

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

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void DisplayName();

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UZeroDialogueComponent> DialogueComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UZeroNPCWidgetComponent> WidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroNPCNameWidget> NPCNameClass;
};
