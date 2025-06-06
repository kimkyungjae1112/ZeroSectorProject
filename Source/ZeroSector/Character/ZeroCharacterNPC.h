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

public:
	void DisplayInterviewMark(FString Name);
	void UnDisplayInterviewMark(FString Name);

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

// NPC 움직임 통제
private:
	void StartRandomPatrolPause();
	void PauseMovement();
	void ResumeMovement();

	FTimerHandle PatrolPauseTimer;
	FTimerHandle PatrolResumeTimer;


private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UZeroDialogueComponent> DialogueComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UZeroNPCWidgetComponent> WidgetComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TSubclassOf<UZeroNPCNameWidget> NPCNameClass;

	UPROPERTY(EditDefaultsOnly, Category = "Name")
	FName DisplayNPCName;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> InterviewWidgetComp;

	float WidgetYawRotation{ 0.0f };
};
