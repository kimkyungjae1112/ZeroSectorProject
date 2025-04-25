// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroMainMenuWidget.generated.h"

class UButton;
class UCanvasPanel;
class USlider;
class UComboBoxString;

UCLASS()
class ZEROSECTOR_API UZeroMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UFUNCTION()
    void OnStartButtonClicked();

    UFUNCTION()
    void OnQuitButtonClicked();

    UFUNCTION()
    void OnOptionButtonClicked();

    UFUNCTION()
    void OnOptionExitButtonClicked();

    UFUNCTION()
    void OnVolumeChanged(float Value);

    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);

    UFUNCTION()
    void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);

    UFUNCTION()
    void OnApplySettingsClicked();


    void InitializeResolutionOptions();
    void InitializeWindowModeOptions();

       
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

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> VolumeSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> WindowModeComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ApplySettingButton;

    /*
    FString PendingResolution;
    FString PendingWindowMode;
    float PendingVolume = 0.5f;

    FString CurrentResolution;
    FString CurrentWindowMode;
    float CurrentVolume = 0.5f; */
};

