// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroMainMenuWidget.h"
#include "ZeroMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "GameFramework/GameUserSettings.h"

void UZeroMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 초기 상태 저장
    CurrentResolution = "1920x1080";
    CurrentWindowMode = "Fullscreen";
    CurrentVolume = 0.5f;

    PendingResolution = CurrentResolution;
    PendingWindowMode = CurrentWindowMode;
    PendingVolume = CurrentVolume;

    USoundBase* BGM = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Sound/FPS_Menu_Music_Vol_1/Cues/juanjo_-_FPS_Menu_Music_Theme_4_Cue.juanjo_-_FPS_Menu_Music_Theme_4_Cue'"));
    if (BGM)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), BGM);
    }

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnStartButtonClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnQuitButtonClicked);
    }

    if (OptionButton)
    {
        OptionButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnOptionButtonClicked);
    }

    if (OptionExitButton)
    {
        OptionExitButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnOptionExitButtonClicked);
    }

    if (VolumeSlider)
    {
        VolumeSlider->OnValueChanged.AddDynamic(this, &UZeroMainMenuWidget::OnVolumeChanged);
        VolumeSlider->SetValue(CurrentVolume);
    }

    if (ResolutionComboBox)
    {
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UZeroMainMenuWidget::OnResolutionChanged);
        InitializeResolutionOptions();
    }

    if (WindowModeComboBox)
    {
        WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UZeroMainMenuWidget::OnWindowModeChanged);
        InitializeWindowModeOptions();
    }

    if (ApplySettingButton)
    {
        ApplySettingButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnApplySettingsClicked);
        ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroMainMenuWidget::OnStartButtonClicked()
{
    UGameplayStatics::OpenLevel(this, "TestMap");
}

void UZeroMainMenuWidget::OnQuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UZeroMainMenuWidget::OnOptionButtonClicked()
{
    if (OptionPanel)
    {
        const bool bIsVisible = OptionPanel->GetVisibility() == ESlateVisibility::Visible;
        OptionPanel->SetVisibility(bIsVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
    }

    if (ApplySettingButton)
    {
        ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroMainMenuWidget::OnOptionExitButtonClicked()
{
    if (OptionPanel)
    {
        OptionPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroMainMenuWidget::OnVolumeChanged(float Value)
{
    PendingVolume = Value;

    static const FString Path = TEXT("/Script/Engine.SoundClass'/Game/Sound/BGM_SoundClass.BGM_SoundClass'");
    if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *Path))
    {
        SoundClass->Properties.Volume = FMath::Clamp(Value, 0.f, 1.f);
    }

    if (!FMath::IsNearlyEqual(PendingVolume, CurrentVolume, 0.01f) && ApplySettingButton)
    {
        ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UZeroMainMenuWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    PendingResolution = SelectedItem;

    if (PendingResolution != CurrentResolution && ApplySettingButton)
    {
        ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UZeroMainMenuWidget::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    PendingWindowMode = SelectedItem;

    if (PendingWindowMode != CurrentWindowMode && ApplySettingButton)
    {
        ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
    }
}

void UZeroMainMenuWidget::OnApplySettingsClicked()
{
    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        FString WidthStr, HeightStr;
        if (PendingResolution.Split("x", &WidthStr, &HeightStr))
        {
            int32 Width = FCString::Atoi(*WidthStr);
            int32 Height = FCString::Atoi(*HeightStr);
            Settings->SetScreenResolution(FIntPoint(Width, Height));
        }

        if (PendingWindowMode == "Fullscreen")
            Settings->SetFullscreenMode(EWindowMode::Fullscreen);
        else if (PendingWindowMode == "Windowed")
            Settings->SetFullscreenMode(EWindowMode::Windowed);
        else if (PendingWindowMode == "Borderless")
            Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);

        Settings->ApplySettings(true);
    }

    static const FString Path = TEXT("/Script/Engine.SoundClass'/Game/Sound/BGM_SoundClass.BGM_SoundClass'");
    if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *Path))
    {
        SoundClass->Properties.Volume = FMath::Clamp(PendingVolume, 0.f, 1.f);
    }

    CurrentResolution = PendingResolution;
    CurrentWindowMode = PendingWindowMode;
    CurrentVolume = PendingVolume;

    if (ApplySettingButton)
    {
        ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);
    }

    UE_LOG(LogTemp, Warning, TEXT("설정 적용 완료: %s / %s / 볼륨 %.2f"), *CurrentResolution, *CurrentWindowMode, CurrentVolume);
}

void UZeroMainMenuWidget::InitializeResolutionOptions()
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

    ResolutionComboBox->SetSelectedOption("1920x1080");
}

void UZeroMainMenuWidget::InitializeWindowModeOptions()
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

    WindowModeComboBox->SetSelectedOption("Fullscreen");
}