// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroMainMenuWidget.generated.h"

class UButton;
class UCanvasPanel;

UCLASS()
class ZEROSECTOR_API UZeroMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeOnInitialized() override;

protected:
    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnQuitButtonClicked();

    UFUNCTION()
    void OnOptionButtonClicked();

    UFUNCTION()
    void OnOptionExitButtonClicked();

       
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> QuitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OptionButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> OptionPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OptionExitButton;
};

