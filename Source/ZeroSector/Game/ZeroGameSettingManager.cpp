// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroGameSettingManager.h"
#include "Sound/SoundClass.h"
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
}

void UZeroGameSettingManager::ApplySettings()
{

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

}

