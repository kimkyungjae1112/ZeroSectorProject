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

    FString GetResolution() const { return Resolution; }
    FString GetWindowMode() const { return WindowMode; }
    float GetVolume() const { return Volume; }

private:
    FString Resolution = TEXT("1920x1080");
    FString WindowMode = TEXT("Fullscreen");
    float Volume = 0.5f;
};
