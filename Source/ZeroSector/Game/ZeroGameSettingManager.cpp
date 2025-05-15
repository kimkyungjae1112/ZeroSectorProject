// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroGameSettingManager.h"
#include "Sound/SoundClass.h"
#include "Game/ZeroGameModeBase.h"
#include "Components/AudioComponent.h"
#include "GameFramework/GameUserSettings.h"

void UZeroGameSettingManager::Init()
{
}

void UZeroGameSettingManager::SetResolution(const FString& InResolution)
{
    Resolution = InResolution;

    UE_LOG(LogTemp, Warning, TEXT("SetResolution 호출됨: %s"), *InResolution);
}

void UZeroGameSettingManager::SetWindowMode(const FString& InWindowMode)
{
    WindowMode = InWindowMode;
}

void UZeroGameSettingManager::SetVolume(float InVolume)
{
    Volume = FMath::Clamp(InVolume, 0.f, 1.f);

    static const FString Path = TEXT("/Script/Engine.SoundClass'/Game/Sound/BGM_SoundClass.BGM_SoundClass'");
    if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *Path))
    {
        SoundClass->Properties.Volume = Volume;
    }

    if (UWorld* World = GEngine->GetWorld())
    {
        AZeroGameModeBase* GM = Cast<AZeroGameModeBase>(World->GetAuthGameMode());
        if (GM && GM->BGMAudioComponent)
        {
            GM->BGMAudioComponent->SetVolumeMultiplier(Volume);
        }
    }
}

void UZeroGameSettingManager::SetSFX(float InSFX)
{
    SFXVolume = FMath::Clamp(InSFX, 0.f, 1.f);

    static const FString Path = TEXT("/Script/Engine.SoundClass'/Game/Sound/SFX_SoundClass.SFX_SoundClass'");
    if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *Path))
    {
        SoundClass->Properties.Volume = SFXVolume;
    }

    if (UWorld* World = GEngine->GetWorld())
    {
        AZeroGameModeBase* GM = Cast<AZeroGameModeBase>(World->GetAuthGameMode());
        if (GM && GM->SFXAudioComponent)
        {
            GM->SFXAudioComponent->SetVolumeMultiplier(SFXVolume);
        }
    }
}

void UZeroGameSettingManager::SetTempVolume(float InVolume)
{
    TempVolume = FMath::Clamp(InVolume, 0.f, 1.f);
}

void UZeroGameSettingManager::SetTempSFX(float InSFX)
{
    TempSFXVolume = FMath::Clamp(InSFX, 0.f, 1.f);
}

void UZeroGameSettingManager::SetTempResolution(const FString& InRes)
{
    TempResolution = InRes;
}

void UZeroGameSettingManager::SetTempWindowMode(const FString& InMode)
{
    TempWindowMode = InMode;
}

void UZeroGameSettingManager::PreviewTempVolume(float InVolume)
{
    TempVolume = FMath::Clamp(InVolume, 0.f, 1.f);

    // 실제로 반영 (SoundClass)
    static const FString Path = TEXT("/Script/Engine.SoundClass'/Game/Sound/BGM_SoundClass.BGM_SoundClass'");
    if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *Path))
    {
        SoundClass->Properties.Volume = TempVolume;
    }

    if (UWorld* World = GEngine->GetWorld())
    {
        AZeroGameModeBase* GM = Cast<AZeroGameModeBase>(World->GetAuthGameMode());
        if (GM && GM->BGMAudioComponent)
        {
            GM->BGMAudioComponent->SetVolumeMultiplier(TempVolume);
        }
    }
}

void UZeroGameSettingManager::PreviewTempSFX(float InSFX)
{
    TempSFXVolume = FMath::Clamp(InSFX, 0.f, 1.f);

    static const FString Path = TEXT("/Script/Engine.SoundClass'/Game/Sound/SFX_SoundClass.SFX_SoundClass'");
    if (USoundClass* SoundClass = LoadObject<USoundClass>(nullptr, *Path))
    {
        SoundClass->Properties.Volume = TempSFXVolume;
    }

    if (UWorld* World = GEngine->GetWorld())
    {
        AZeroGameModeBase* GM = Cast<AZeroGameModeBase>(World->GetAuthGameMode());
        if (GM && GM->SFXAudioComponent)
        {
            GM->SFXAudioComponent->SetVolumeMultiplier(TempSFXVolume);
        }
    }
}



void UZeroGameSettingManager::ApplySettings()
{
    Resolution = TempResolution;
    WindowMode = TempWindowMode;
    Volume = TempVolume;
    SFXVolume = TempSFXVolume;

    if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
    {
        FString WidthStr, HeightStr;
        if (Resolution.Split("x", &WidthStr, &HeightStr))
        {
            int32 Width = FCString::Atoi(*WidthStr);
            int32 Height = FCString::Atoi(*HeightStr);
            Settings->SetScreenResolution(FIntPoint(Width, Height));

            if (GEngine && GEngine->GameViewport && GEngine->GameViewport->GetWorld())
            {
                FString Command = FString::Printf(TEXT("r.SetRes %dx%df"), Width, Height);
                GEngine->Exec(GEngine->GameViewport->GetWorld(), *Command);
            }
        }

        if (WindowMode == "Fullscreen")
            Settings->SetFullscreenMode(EWindowMode::Fullscreen);
        else if (WindowMode == "Windowed")
            Settings->SetFullscreenMode(EWindowMode::Windowed);
        else if (WindowMode == "Borderless")
            Settings->SetFullscreenMode(EWindowMode::WindowedFullscreen);

        Settings->ApplySettings(false);
        Settings->SaveSettings();
    }

    {
        static const FString BGMPath = TEXT("/Script/Engine.SoundClass'/Game/Sound/BGM_SoundClass.BGM_SoundClass'");
        if (USoundClass* BGM = LoadObject<USoundClass>(nullptr, *BGMPath))
        {
            BGM->Properties.Volume = Volume;
        }

        static const FString SFXPath = TEXT("/Script/Engine.SoundClass'/Game/Sound/SFX_SoundClass.SFX_SoundClass'");
        if (USoundClass* SFX = LoadObject<USoundClass>(nullptr, *SFXPath))
        {
            SFX->Properties.Volume = SFXVolume;
        }
    }
}

void UZeroGameSettingManager::ResetTempSettings()
{
    TempVolume = Volume;
    TempSFXVolume = SFXVolume;

    TempResolution = Resolution;
    TempWindowMode = WindowMode;

    static const FString BGMPath = TEXT("/Script/Engine.SoundClass'/Game/Sound/BGM_SoundClass.BGM_SoundClass'");
    if (USoundClass* BGM = LoadObject<USoundClass>(nullptr, *BGMPath))
    {
        BGM->Properties.Volume = Volume;  
    }

    static const FString SFXPath = TEXT("/Script/Engine.SoundClass'/Game/Sound/SFX_SoundClass.SFX_SoundClass'");
    if (USoundClass* SFX = LoadObject<USoundClass>(nullptr, *SFXPath))
    {
        SFX->Properties.Volume = SFXVolume;  
    }

    if (UWorld* World = GEngine->GetWorld())
    {
        AZeroGameModeBase* GM = Cast<AZeroGameModeBase>(World->GetAuthGameMode());
        if (GM)
        {
            if (GM->BGMAudioComponent)
                GM->BGMAudioComponent->SetVolumeMultiplier(Volume);

            if (GM->SFXAudioComponent)
                GM->SFXAudioComponent->SetVolumeMultiplier(SFXVolume);
        }
    }
}



