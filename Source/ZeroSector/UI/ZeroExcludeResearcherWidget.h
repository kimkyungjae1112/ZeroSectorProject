// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroExcludeResearcherWidget.generated.h"

class UButton;
class UTextBlock;
class UCanvasPanel;

DECLARE_DELEGATE(FOnCloseExclude)

UCLASS()
class ZEROSECTOR_API UZeroExcludeResearcherWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCloseExclude OnCloseExclude;

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY()
	FString ExcludedResearcherName;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExcludeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NotExcludeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExRes1Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExRes2Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExRes3Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExRes4Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExRes5Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> ExcludeCheckPopup;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExcludeCheckText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExcludeOKButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExcludeCancelButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExcludeWarningText;

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
	void OnClickExcludeButton();

	UFUNCTION()
	void OnClickNotExcludeButton();

	UFUNCTION()
	void OnClickExcludeOK();

	UFUNCTION()
	void OnClickExcludeCancel();

private:
	int32 ExcludedResearcherIndex = -1;

	void UpdateButtonStyles();

	void HandleResearcherSelection(int32 Index);

	void HideExcludeWarningText();

	UPROPERTY()
	TArray<FString> ResearcherNames;
};

