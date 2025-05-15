// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroPauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroGameSettingManager.h"
#include "Game/ZeroSoundManager.h"
#include "Player/ZeroPlayerController.h"
#include "GameFramework/GameUserSettings.h"

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

    if (SFXSlider && SM)
    {
        SFXSlider->OnValueChanged.AddDynamic(this, &UZeroPauseMenuWidget::OnSFXChanged);
        SFXSlider->SetValue(SM->GetSFXVolume());
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

    UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
    }
}

void UZeroPauseMenuWidget::OnMainMenuButtonClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), false);
    UGameplayStatics::OpenLevel(this, FName("MainMenuMap"));

    UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
    }
}

void UZeroPauseMenuWidget::OnOptionButtonClicked()
{
    if (OptionPanel)
    {
        const bool bVisible = OptionPanel->GetVisibility() == ESlateVisibility::Visible;
        OptionPanel->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

        if (!bVisible)  
        {
            if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
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

        UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
        if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
        {
            UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
        }
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

    UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
    UZeroGameSettingManager* SM = GI ? GI->SettingManager : nullptr;
    SM->ResetTempSettings();

}

void UZeroPauseMenuWidget::OnVolumeChanged(float Value)
{
    if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->PreviewTempVolume(Value);
            ApplySettingButton->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UZeroPauseMenuWidget::OnSFXChanged(float Value)
{
    if (UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance()))
    {
        if (UZeroGameSettingManager* SM = GI->SettingManager)
        {
            SM->PreviewTempSFX(Value);
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
            SM->SetTempResolution(SelectedItem);
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
            SM->SetTempWindowMode(SelectedItem);
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

            UE_LOG(LogTemp, Log, TEXT("퍼즈 메뉴 설정 적용됨: %s / %s / 볼륨 %.2f / 효과음 %.2f"),
                *SM->GetResolution(), *SM->GetWindowMode(), SM->GetVolume(), SM->GetSFXVolume());
        }
    }
}

void UZeroPauseMenuWidget::InitializeResolutionOptions()
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