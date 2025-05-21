// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "ZeroCharacterNPC.generated.h"

class UZeroDialogueComponent;
class UZeroNPCWidgetComponent;
class UZeroNPCNameWidget;
class AZeroAIControllerNPC;

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

	/* IZeroClassIdentifierInterface Implement */
	FORCEINLINE virtual FName GetNPCName() const override { return DisplayNPCName; }

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


// 유틸리티
private:
	AZeroAIControllerNPC* GetAIController() const;

// 다음날 이동 함수 등록
private:
	void MoveNextDayStartLoc(const FVector& InLocation);

// NPC 이름 호출
private:
	void DisplayName();

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UZeroDialogueComponent> DialogueComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UZeroNPCWidgetComponent> WidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroNPCNameWidget> NPCNameClass;

	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FName DisplayNPCName;
};
