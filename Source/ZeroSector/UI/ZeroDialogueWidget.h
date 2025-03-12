// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroDialogueWidget.generated.h"

class UTextBlock;
class UScrollBox;

UCLASS()
class ZEROSECTOR_API UZeroDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroDialogueWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UScrollBox* GetScrollBox() const;

	void SetDialogueText(const FText& InSpeaker, const FText& InDialogue);
	void UpdateDialogue(const FText& InDialogue);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> SpeakerTextBlock;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> DialogueTextBlock;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UScrollBox> OptionScrollBox;
};
