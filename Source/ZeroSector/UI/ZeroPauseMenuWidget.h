// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroPauseMenuWidget.generated.h"

class UButton;

UCLASS()
class ZEROSECTOR_API UZeroPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeOnInitialized() override;

protected:
    UFUNCTION()
    void OnResumeButtonClicked();

    UFUNCTION()
    void OnMainMenuButtonClicked();

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ResumeButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> MainMenuButton;
};
