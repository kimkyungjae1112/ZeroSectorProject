// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZeroPlayerController.h"
#include "Data/ZeroSingleton.h"
#include "UI/ZeroHUDWidget.h"
#include "UI/ZeroAfternoonHUDWidget.h"
#include "UI/ZeroLoseScreenWidget.h"
#include "UI/ZeroEndingVideoWidget.h"
#include "Game/ZeroGameModeBase.h"
#include "Kismet/GameplayStatics.h"
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
	static ConstructorHelpers::FClassFinder<UZeroEndingVideoWidget> EndingCinematicClassRef(TEXT("/Game/Cinematic/Ending/WBP_EndingCinematic.WBP_EndingCinematic_C"));
	if (EndingCinematicClassRef.Class)
	{
		EndingCinematicClass = EndingCinematicClassRef.Class;
	}
}

void AZeroPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		if (AZeroGameModeBase::Day == 7)
		{
			// 시네마틱 재생
			// 메인 메뉴로 이동
			if (UZeroSingleton::Get().SelectedResearcherName == TEXT("Vaccine"))
			{
				// 해피 엔딩
				FTimerHandle EndingTimer;
				GetWorld()->GetTimerManager().SetTimer(EndingTimer, [&]()
					{
						UZeroEndingVideoWidget* EndingWidget = CreateWidget<UZeroEndingVideoWidget>(this, EndingCinematicClass);
						if (EndingWidget)
						{
							EndingWidget->AddToViewport();
						}
					}, 2.5f, false);
			}
			else if (UZeroSingleton::Get().SelectedResearcherName == TEXT("Criminal"))
			{
				// 배드 엔딩
				FTimerHandle EndingTimer;
				GetWorld()->GetTimerManager().SetTimer(EndingTimer, [&]()
					{
						UZeroEndingVideoWidget* EndingWidget = CreateWidget<UZeroEndingVideoWidget>(this, EndingCinematicClass);
						if (EndingWidget)
						{
							EndingWidget->AddToViewport();
						}
					}, 2.5f, false);
			}
			else
			{
				// 일반 엔딩
				FTimerHandle EndingTimer;
				GetWorld()->GetTimerManager().SetTimer(EndingTimer, [&]()
					{
						UZeroEndingVideoWidget* EndingWidget = CreateWidget<UZeroEndingVideoWidget>(this, EndingCinematicClass);
						if (EndingWidget)
						{
							EndingWidget->AddToViewport();
						}
					}, 2.5f, false);
			}

			return;
		}

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
				// GameModeBase BeginPlay 에서 RestartLevel 함수 바인딩
				// Player 클래스에서 Player의 체력 초기화 함수 바인딩
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

float AZeroPlayerController::GetCurrentSpread() const
{
	return HUDWidgetPtr->GetCurrentSpread();
}

void AZeroPlayerController::CrosshairSpread(float Amount) const
{
	HUDWidgetPtr->IncreaseSpread(Amount);
}

void AZeroPlayerController::HitCrosshair() const
{
	HUDWidgetPtr->HitCrosshair();
}

void AZeroPlayerController::UnHitCrosshair() const
{
	HUDWidgetPtr->UnHitCrosshair();
}

void AZeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputModeGameOnly();

	if (AfternoonHUDWidgetClass)
	{
		AfternoonHUDWidgetPtr = CreateWidget<UZeroAfternoonHUDWidget>(this, AfternoonHUDWidgetClass);
	}
	if (HUDWidgetClass)
	{
		HUDWidgetPtr = CreateWidget<UZeroHUDWidget>(this, HUDWidgetClass);
		if (HUDWidgetPtr && !PrologFlag)
		{
			HUDWidgetPtr->AddToViewport();
		}
	}

}


