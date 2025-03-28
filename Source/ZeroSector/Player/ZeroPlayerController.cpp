// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZeroPlayerController.h"
#include "UI/ZeroHUDWidget.h"

AZeroPlayerController::AZeroPlayerController()
{
	static ConstructorHelpers::FClassFinder<UZeroHUDWidget> HUDWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> BlurWidgetRef(TEXT("/Game/Blueprints/UI/WBP_Blur.WBP_Blur_C"));
	if (BlurWidgetRef.Succeeded())
	{
		BlurWidgetClass = BlurWidgetRef.Class;
	}
}

void AZeroPlayerController::InputModeGameOnly()
{
	bShowMouseCursor = false;
	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
}

void AZeroPlayerController::InputModeUIOnly()
{
	bShowMouseCursor = true;
	FInputModeUIOnly UIOnly;
	SetInputMode(UIOnly);
}

void AZeroPlayerController::InputModeGameAndUI()
{
	bShowMouseCursor = true;
	FInputModeGameAndUI GameAndUI;
	SetInputMode(GameAndUI);
}

void AZeroPlayerController::ApplyBlurEffect()
{
	if (!BlurWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BLUR"))
	}

	if (!BlurWidgetInstance)
	{
		BlurWidgetInstance = CreateWidget<UUserWidget>(this, BlurWidgetClass);
	}

	if (BlurWidgetInstance && !BlurWidgetInstance->IsInViewport())
	{
		BlurWidgetInstance->AddToViewport(-100); 
	}
}

void AZeroPlayerController::RemoveBlurEffect()
{
	if (BlurWidgetInstance && BlurWidgetInstance->IsInViewport())
	{
		BlurWidgetInstance->RemoveFromParent();
	}
}


void AZeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputModeGameOnly();

	HUDWidgetPtr = CreateWidget<UZeroHUDWidget>(this, HUDWidgetClass);
	if (HUDWidgetPtr)
	{
		HUDWidgetPtr->AddToViewport();
	}
}


