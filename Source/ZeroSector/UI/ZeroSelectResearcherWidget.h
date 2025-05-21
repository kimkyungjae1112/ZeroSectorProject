// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroSelectResearcherWidget.generated.h"

class UButton;
class UTextBlock;
class UCanvasPanel;
class UZeroGameInstance;


DECLARE_DELEGATE(FOnCloseSelect)

UCLASS()
class ZEROSECTOR_API UZeroSelectResearcherWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	FOnCloseSelect OnCloseSelect;

public:
	UPROPERTY()
	FString SelectedResearcherName;


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelRes1Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelRes2Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelRes3Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelRes4Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelRes5Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> SelectCheckPopup;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectCheckText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectOKButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectCancelButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectWarningText;

	FTimerHandle WarningTextTimerHandle;

	UFUNCTION()
	void OnClickResearcher1();

	UFUNCTION()
	void OnClickResearcher2();

	UFUNCTION()
	void OnClickResearcher3();

	UFUNCTION()
	void OnClickResearcher4();

	UFUNCTION()
	void OnClickResearcher5();

	UFUNCTION()
	void OnClickSelectButton();

	UFUNCTION()
	void OnClickSelectOK();

	UFUNCTION()
	void OnClickSelectCancel();

private:
	int32 SelectedResearcherIndex = -1;

	void UpdateButtonStyles();

	void HandleResearcherSelection(int32 Index);

	void HideSelectWarningText();

	TArray<FString> ResearcherNames;

	UPROPERTY()
	TObjectPtr<UZeroGameInstance> GI;
};

