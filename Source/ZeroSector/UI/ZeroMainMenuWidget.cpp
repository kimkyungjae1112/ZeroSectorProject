// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroMainMenuWidget.h"
#include "ZeroMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"


void UZeroMainMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

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
}

void UZeroMainMenuWidget::OnOptionExitButtonClicked()
{
    if (OptionPanel)
    {
        OptionPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}
