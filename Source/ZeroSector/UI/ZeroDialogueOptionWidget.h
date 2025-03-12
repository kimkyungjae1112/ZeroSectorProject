// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ZeroDialogueDataTable.h"
#include "ZeroDialogueOptionWidget.generated.h"

class UButton;
class UTextBlock;
class UZeroDialogueComponent;

UCLASS()
class ZEROSECTOR_API UZeroDialogueOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UZeroDialogueOptionWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	
public:
	void SetDialogueComp(UZeroDialogueComponent* InDialogueComp);
	void SetDialogueOptionText(const FText& InText);

	FZeroDialogueDataTable DialogueTableInOption;

private:
	UFUNCTION()
	void OnClickedOption();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> DialogueOptionButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> DialogueOptionTextBlock;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UZeroDialogueComponent> DialogueComp;
};
