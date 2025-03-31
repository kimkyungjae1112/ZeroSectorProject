// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroMainMenuGameMode.h"
#include "UI/ZeroMainMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AZeroMainMenuGameMode::AZeroMainMenuGameMode()
{
    static ConstructorHelpers::FClassFinder<UZeroMainMenuWidget> MenuWidgetBPClass(TEXT("/Game/Blueprints/UI/WBP_MainMenu"));
    if (MenuWidgetBPClass.Succeeded())
    {
        MainMenuWidgetClass = MenuWidgetBPClass.Class;
    }

}

void AZeroMainMenuGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass)
    {
        UZeroMainMenuWidget* Menu = CreateWidget<UZeroMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
        if (Menu)
        {
            Menu->AddToViewport();
            APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            if (PC)
            {
                PC->SetShowMouseCursor(true);
                PC->SetInputMode(FInputModeUIOnly());
            }
        }
    }
}