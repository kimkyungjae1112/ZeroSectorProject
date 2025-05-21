// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Sound/SoundClass.h"
#include "GameFramework/GameUserSettings.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Game/ZeroGameSettingManager.h"
#include "GameFramework/GameUserSettings.h"

void UZeroMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GI = Cast<UZeroGameInstance>(GetGameInstance());
    UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;

    USoundBase* BGM = LoadObject<USoundBase>(nullptr, TEXT("SoundCue'/Game/Sound/FPS_Menu_Music_Vol_1/Cues/juanjo_-_FPS_Menu_Music_Theme_4_Cue.juanjo_-_FPS_Menu_Music_Theme_4_Cue'"));
    if (BGM)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), BGM);
    }

    if (StartButton)
        StartButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnStartButtonClicked);

    if (QuitButton)
        QuitButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnQuitButtonClicked);

    if (OptionButton)
        OptionButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnOptionButtonClicked);

    if (OptionExitButton)
        OptionExitButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnOptionExitButtonClicked);

    if (VolumeSlider && SM)
    {
        VolumeSlider->OnValueChanged.AddDynamic(this, &UZeroMainMenuWidget::OnVolumeChanged);
        VolumeSlider->SetValue(SM->GetVolume());
    }

    if (SFXSlider && SM)
    {
        SFXSlider->OnValueChanged.AddDynamic(this, &UZeroMainMenuWidget::OnSFXChanged);
        SFXSlider->SetValue(SM->GetSFXVolume());
    }

    if (ResolutionComboBox && SM)
    {
        ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UZeroMainMenuWidget::OnResolutionChanged);
        InitializeResolutionOptions();
        ResolutionComboBox->SetSelectedOption(SM->GetResolution());
    }

    if (WindowModeComboBox && SM)
    {
        WindowModeComboBox->OnSelectionChanged.AddDynamic(this, &UZeroMainMenuWidget::OnWindowModeChanged);
        InitializeWindowModeOptions();
        WindowModeComboBox->SetSelectedOption(SM->GetWindowMode());
    }

    if (ApplySettingButton)
    {
        ApplySettingButton->OnClicked.AddDynamic(this, &UZeroMainMenuWidget::OnApplySettingsClicked);
        ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UZeroMainMenuWidget::OnStartButtonClicked()
{
    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
    }

    UGameplayStatics::OpenLevel(this, "KJMap");
}

void UZeroMainMenuWidget::OnQuitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void UZeroMainMenuWidget::OnOptionButtonClicked()
{
    if (OptionPanel)
    {
        const bool bVisible = OptionPanel->GetVisibility() == ESlateVisibility::Visible;
        OptionPanel->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

        if (!bVisible)
        {
            if (GI)
            {
                if (UZeroGameSettingManager* SM = GI->SettingManager)
                {
                    if (VolumeSlider) VolumeSlider->SetValue(SM->GetTempVolume());
                    if (SFXSlider) SFXSlider->SetValue(SM->GetTempSFXVolume());
                    if (ResolutionComboBox) ResolutionComboBox->SetSelectedOption(SM->GetTempResolution());
                    if (WindowModeComboBox) WindowModeComboBox->SetSelectedOption(SM->GetTempWindowMode());
                }
            }
        }
    }

    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
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

    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
    }

    UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;
    SM->ResetTempSettings();
}

void UZeroMainMenuWidget::OnVolumeChanged(float Value)
{
    if (GI)
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->PreviewTempVolume(Value);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroMainMenuWidget::OnSFXChanged(float Value)
{
    if (GI)
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->PreviewTempSFX(Value);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroMainMenuWidget::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (GI)
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->SetTempResolution(SelectedItem);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroMainMenuWidget::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectInfo)
{
    if (GI)
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->SetTempWindowMode(SelectedItem);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroMainMenuWidget::OnApplySettingsClicked()
{
    if (GI)
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->ApplySettings();
            ApplySettingButton->SetVisibility(ESlateVisibility::Collapsed);

            UE_LOG(LogTemp, Warning, TEXT("설정 적용 완료: %s / %s / 볼륨 %.2f"),
                *SM->GetResolution(), *SM->GetWindowMode(), SM->GetVolume());
        }
    }

    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
    }
}

void UZeroMainMenuWidget::InitializeResolutionOptions()
{
    if (!ResolutionComboBox) return;

    // 실제 데스크탑 해상도 가져오기
    FIntPoint DesktopRes = UGameUserSettings::GetGameUserSettings()->GetDesktopResolution();
    int32 ScreenWidth = DesktopRes.X;
    int32 ScreenHeight = DesktopRes.Y;

    TArray<FString> Resolutions = {
        TEXT("1280x720"),
        TEXT("1600x900"),
        TEXT("1920x1080"),
        TEXT("2560x1440"),
        TEXT("3840x2160")
    };

    for (const FString& Res : Resolutions)
    {
        FString WidthStr, HeightStr;
        if (Res.Split(TEXT("x"), &WidthStr, &HeightStr))
        {
            int32 W = FCString::Atoi(*WidthStr);
            int32 H = FCString::Atoi(*HeightStr);

            if (W <= ScreenWidth && H <= ScreenHeight)
            {
                ResolutionComboBox->AddOption(Res);
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("해상도 %s 제외됨: 데스크탑 화면보다 큼 (%d x %d)"), *Res, ScreenWidth, ScreenHeight);
            }
        }
    }
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
}
