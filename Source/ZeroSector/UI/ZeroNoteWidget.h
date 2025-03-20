// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroNoteWidget.generated.h"

struct FZeroProvisoDataTable;

/**
 * 
 */
UCLASS()
class ZEROSECTOR_API UZeroNoteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNoteInfo(const FString& ProvisoName, const FString& Description);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* ProvisoListBox; 

	void ShowWidget();

	void AddProvisoToUI(const FString& ProvisoName, const FString& Description);

	
};
