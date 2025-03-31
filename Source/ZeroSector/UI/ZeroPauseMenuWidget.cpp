// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroPauseMenuWidget.h"
#include "Components/Button.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UZeroPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ResumeButton)
        ResumeButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnResumeButtonClicked);

    if (MainMenuButton)
        MainMenuButton->OnClicked.AddDynamic(this, &UZeroPauseMenuWidget::OnMainMenuButtonClicked);
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