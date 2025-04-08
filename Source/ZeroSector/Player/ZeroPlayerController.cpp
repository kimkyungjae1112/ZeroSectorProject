// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZeroPlayerController.h"
#include "UI/ZeroHUDWidget.h"
#include "UI/ZeroAfternoonHUDWidget.h"
#include "UI/ZeroLoseScreenWidget.h"
#include "ZeroSector.h"

AZeroPlayerController::AZeroPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> BlurWidgetRef(TEXT("/Game/Blueprints/UI/WBP_Blur.WBP_Blur_C"));
	if (BlurWidgetRef.Succeeded())
	{
		BlurWidgetClass = BlurWidgetRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UZeroHUDWidget> HUDWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_HUD.WBP_HUD_C"));
	if (HUDWidgetClassRef.Class)
	{
		HUDWidgetClass = HUDWidgetClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UZeroAfternoonHUDWidget> AfternoonHUDWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_AfternoonHUD.WBP_AfternoonHUD_C"));
	if (AfternoonHUDWidgetClassRef.Class)
	{
		AfternoonHUDWidgetClass = AfternoonHUDWidgetClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UUserWidget> WinScreenClassRef(TEXT("/Game/Blueprints/UI/WBP_WinScreen.WBP_WinScreen_C"));
	if (WinScreenClassRef.Class)
	{
		WinScreenClass = WinScreenClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UZeroLoseScreenWidget> LoseScreenClassRef(TEXT("/Game/Blueprints/UI/WBP_LoseScreen.WBP_LoseScreen_C"));
	if (LoseScreenClassRef.Class)
	{
		LoseScreenClass = LoseScreenClassRef.Class;
	}
}

void AZeroPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		WinScreenPtr = CreateWidget<UUserWidget>(this, WinScreenClass);
		if (WinScreenPtr) WinScreenPtr->AddToViewport();

		FTimerHandle WinTimer;
		GetWorld()->GetTimerManager().SetTimer(WinTimer, [&]()
			{
				OnClearZombie.Broadcast();

				WinScreenPtr->RemoveFromParent();
			}, 3.f, false);
	}
	else
	{
		LoseScreenPtr = CreateWidget<UZeroLoseScreenWidget>(this, LoseScreenClass);
		if (LoseScreenPtr)
		{
			LoseScreenPtr->AddToViewport();
			LoseScreenPtr->FadeInPlay();
		}

		HUDWidgetPtr->RemoveFromParent();

		FTimerHandle LoseTimer;
		GetWorld()->GetTimerManager().SetTimer(LoseTimer, [&]()
			{
				OnNonClearZmobie.Broadcast();
			}, 3.f, false);
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

void AZeroPlayerController::ATHUD_Display()
{
	if (HUDWidgetPtr->IsInViewport())
	{
		HUDWidgetPtr->RemoveFromParent();
	}
	if (AfternoonHUDWidgetPtr && !AfternoonHUDWidgetPtr->IsInViewport())
	{
		AfternoonHUDWidgetPtr->AddToViewport();
	}
}

void AZeroPlayerController::NightHUD_Display()
{
	if (AfternoonHUDWidgetPtr->IsInViewport())
	{
		AfternoonHUDWidgetPtr->RemoveFromParent();
	}
	if (HUDWidgetPtr && !HUDWidgetPtr->IsInViewport())
	{
		HUDWidgetPtr->AddToViewport();
	}
}


void AZeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputModeGameOnly();

	if (AfternoonHUDWidgetClass)
	{
		AfternoonHUDWidgetPtr = CreateWidget<UZeroAfternoonHUDWidget>(this, AfternoonHUDWidgetClass);
		if (AfternoonHUDWidgetPtr)
		{
			AfternoonHUDWidgetPtr->AddToViewport();
		}
	}
	if (HUDWidgetClass)
	{
		HUDWidgetPtr = CreateWidget<UZeroHUDWidget>(this, HUDWidgetClass);
	}
}


