// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Input/ZeroInputBaseComponent.h"
#include "ZeroInputAfternoonComponent.generated.h"

struct FInputActionValue;
class AZeroGimmick;
class IZeroDialogueInterface;

UCLASS()
class ZEROSECTOR_API UZeroInputAfternoonComponent : public UZeroInputBaseComponent
{
	GENERATED_BODY()
	
public:
	UZeroInputAfternoonComponent();

	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Run() override;
	virtual void Walk() override;

	virtual void InteractBeam() override;
	virtual void InteractProcess(const FHitResult& InHitResult, bool bIsHit) override;
	virtual void InteractBeamReachedProviso(AActor* InHitActor) override;
	virtual void DialogueInteract() override;
	virtual void ProvisoInteract() override;
	virtual void OperationBoardInteract() override;
	virtual void ToggleNote() override;
	virtual void PauseMenu() override;
	virtual void ExcludeResearcher() override;
	virtual void SelectResearcher() override;

protected:
	virtual void BeginPlay() override;

private:
	void SetDefaultMovement();
	void SetDialogueMovement();

private:
	UPROPERTY(EditAnywhere, Category = "Interact")
	float DetectDistance;

	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<AZeroGimmick> InteractedGimmick;

	IZeroDialogueInterface* DialogueInterface;

	bool bIsNoteToggle;
};
