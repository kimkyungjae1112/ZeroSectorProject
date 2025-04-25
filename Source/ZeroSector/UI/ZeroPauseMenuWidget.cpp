// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroPauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroGameSettingManager.h"
#include "Player/ZeroPlayerController.h"

void UZeroPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
    UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;

    if (ResumeButton)
        ResumeButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnResumeButtonClicked);

    if (MainMenuButton)
        MainMenuButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnMainMenuButtonClicked);

    if (OptionButton)
        OptionButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnOptionButtonClicked);

    if (OptionExitButton)
        OptionExitButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnOptionExitButtonClicked);

    if (ApplySettingButton)
    {
        ApplySettingButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnApplySettingsClicked);
        ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (VolumeSlider && SM)
    {
        VolumeSlider->OnValueChanged.AddDynamic(this, &UZeroPauseMenuWidget::OnVolumeChanged);
        VolumeSlider->SetValue(SM->GetVolume());
    }

    if (ResolutionComboBox && SM)
    {
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UZeroPauseMenuWidget::OnResolutionChanged);
        InitializeResolutionOptions();
        ResolutionComboBox->SetSelectedOption(SM->GetResolution());
    }

    if (WindowModeComboBox && SM)
    {
        WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UZeroPauseMenuWidget::OnWindowModeChanged);
        InitializeWindowModeOptions();
        WindowModeComboBox->SetSelectedOption(SM->GetWindowMode());
    }

    if (OptionPanel)
    {
        OptionPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroPauseMenuWidget::OnResumeButtonClicked()
{
    RemoveFromParent();
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    if (AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetOwningPlayer()))
    {
        PC->InputModeGameOnly();
    }
}

void UZeroPauseMenuWidget::OnMainMenuButtonClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));
}

void UZeroPauseMenuWidget::OnOptionButtonClicked()
{
    if (OptionPanel)
    {
        const bool bVisible = OptionPanel->GetVisibility() == ESlateVisibility::Visible;
        OptionPanel->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }

    if (ApplySettingButton)
    {
        ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroPauseMenuWidget::OnOptionExitButtonClicked()
{
    if (OptionPanel)
    {
        OptionPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroPauseMenuWidget::OnVolumeChanged(float Value)
{
    if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->SetVolume(Value);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroPauseMenuWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->SetResolution(SelectedItem);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroPauseMenuWidget::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->SetWindowMode(SelectedItem);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroPauseMenuWidget::OnApplySettingsClicked()
{
    if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->ApplySettings();
            ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);

            UE_LOG(LogTemp, Log, TEXT("퍼즈 메뉴 설정 적용됨: %s / %s / 볼륨 %.2f"),
                *SM->GetResolution(), *SM->GetWindowMode(), SM->GetVolume());
        }
    }
}

void UZeroPauseMenuWidget::InitializeResolutionOptions()
{
    if (!ResolutionComboBox) return;

    TArray<FString> Resolutions = {
        TEXT("1280x720"),
        TEXT("1600x900"),
        TEXT("1920x1080"),
        TEXT("2560x1440"),
        TEXT("3840x2160")
    };

    for (const FString& Res : Resolutions)
    {
        ResolutionComboBox->AddOption(Res);
    }
}

void UZeroPauseMenuWidget::InitializeWindowModeOptions()
{
    if (!WindowModeComboBox) return;

    TArray<FString> Modes = {
        TEXT("Windowed"),
        TEXT("Fullscreen"),
        TEXT("Borderless")
    };

    for (const FString& Mode : Modes)
    {
        WindowModeComboBox->AddOption(Mode);
    }
}