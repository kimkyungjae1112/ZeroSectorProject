// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroPauseMenuWidget.generated.h"

class UButton;
class USlider;
class UComboBoxString;
class UCanvasPanel;
class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	protected:
    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnResumeButtonClicked();

    UFUNCTION()
    void OnMainMenuButtonClicked();

    UFUNCTION()
    void OnOptionButtonClicked();

    UFUNCTION()
    void OnOptionExitButtonClicked();

    UFUNCTION()
    void OnApplySettingsClicked();

    UFUNCTION()
    void OnVolumeChanged(float Value);

    UFUNCTION()
    void OnSFXChanged(float Value);

    UFUNCTION()
    void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);

    UFUNCTION()
    void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);

    void InitializeResolutionOptions();
    void InitializeWindowModeOptions();


protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ResumeButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MainMenuButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OptionButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OptionExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ApplySettingButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> OptionPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> VolumeSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> SFXSlider;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> ResolutionComboBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UComboBoxString> WindowModeComboBox;


};

