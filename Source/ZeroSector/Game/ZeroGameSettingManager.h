// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ZeroGameSettingManager.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroGameSettingManager : public UObject
{
    GENERATED_BODY()

    public:
    void Init();
    void ApplySettings();

    void SetResolution(const FString& InResolution);
    void SetWindowMode(const FString& InWindowMode);
    void SetVolume(float InVolume);
    void SetSFX(float InSFX);

    FString GetResolution() const { return Resolution; }
    FString GetWindowMode() const { return WindowMode; }
    float GetVolume() const { return Volume; }
    float GetSFXVolume() const { return SFXVolume; }

    void SetTempVolume(float InVolume);
    void SetTempSFX(float InSFX);
    void SetTempResolution(const FString& InRes);
    void SetTempWindowMode(const FString& InMode);

    void PreviewTempVolume(float InVolume);
    void PreviewTempSFX(float InSFX);

    void ResetTempSettings();

    float GetTempVolume() const { return TempVolume; }
    float GetTempSFXVolume() const { return TempSFXVolume; }
    FString GetTempResolution() const { return TempResolution; }
    FString GetTempWindowMode() const { return TempWindowMode; }



private:
    FString Resolution = TEXT("1920x1080");
    FString WindowMode = TEXT("Fullscreen");
    float Volume = 0.5f;
    float SFXVolume = 0.5f;

    float TempVolume = Volume;
    float TempSFXVolume = SFXVolume;
    FString TempResolution = Resolution;
    FString TempWindowMode = WindowMode;
};
