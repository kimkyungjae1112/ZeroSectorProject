// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZeroPlayerController.h"
#include "UI/ZeroHUDWidget.h"
#include "UI/ZeroAfternoonHUDWidget.h"
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
}

void AZeroPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		FTimerHandle WinTimer;
		GetWorld()->GetTimerManager().SetTimer(WinTimer, [&]()
			{
				OnClearZombie.Broadcast();
			}, 3.f, false);
		ZE_LOG(LogZeroSector, Display, TEXT("Player Win"));
	}
	else
	{
		ZE_LOG(LogZeroSector, Display, TEXT("Player Loose"));
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


