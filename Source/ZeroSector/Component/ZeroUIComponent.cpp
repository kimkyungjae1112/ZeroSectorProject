// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroUIComponent.h"
#include "ZeroHeader/ZeroUIHeader.h"
#include "Interface/ZeroUIComponentInterface.h"
#include "Player/ZeroPlayerController.h"
#include "Data/ZeroSingleton.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ZeroPauseMenuWidget.h"
#include "ZeroSector.h"

UZeroUIComponent::UZeroUIComponent()
{
	static ConstructorHelpers::FClassFinder<UZeroPauseMenuWidget> WidgetClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu"));
	if (WidgetClass.Succeeded())
	{
		PauseMenuWidgetClass = WidgetClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TT"));
	}
}


void UZeroUIComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UZeroUIComponent::OperationNextButtonClick()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeGameOnly();
		Interface->ChangeInputMode();
	}

	OnClickOperationNextButton.ExecuteIfBound(OperationWidgetPtr->GetWeaponType());
	OperationWidgetPtr->RemoveFromParent();

	FadeInAndOutDisplay();
}

void UZeroUIComponent::FadeInAndOutDisplay()
{
	FadeInAndOutWidgetPtr = CreateWidget<UZeroFadeInAndOutWidget>(GetWorld(), FadeInAndOutWidgetClass);
	FadeInAndOutWidgetPtr->AddToViewport();
	FadeInAndOutWidgetPtr->FadeInPlay();
	FadeInAndOutWidgetPtr = nullptr;
}

void UZeroUIComponent::ToggleNoteDisplay()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());

	if (bIsNoteToggle)
	{
		if (NoteWidgetPtr && NoteWidgetPtr->IsInViewport())
		{
			NoteWidgetPtr->RemoveFromParent();
		}
		bIsNoteToggle = false;
		Interface->GetOwnerController()->RemoveBlurEffect();
		Interface->GetOwnerController()->InputModeGameOnly();
	}
	else
	{
		if (!NoteWidgetPtr)
		{
			// 최초 한 번만 생성
			NoteWidgetPtr = CreateWidget<UZeroNoteWidget>(GetWorld(), NoteWidgetClass);
			if (!NoteWidgetPtr)
			{
				return;
			}
		}

		if (!NoteWidgetPtr->IsInViewport())
		{
			NoteWidgetPtr->AddToViewport();
		}

		bIsNoteToggle = true;
		Interface->GetOwnerController()->ApplyBlurEffect();
		Interface->GetOwnerController()->InputModeGameAndUI();
	}
}

void UZeroUIComponent::InteractUIDisplay()
{
	if (!ProvisoWidgetPtr)
	{
		if (ProvisoWidgetClass)
		{
			ProvisoWidgetPtr = CreateWidget<UZeroProvisoWidget>(GetWorld(), ProvisoWidgetClass);
		}
	}
	if (ProvisoWidgetPtr)
	{
		ProvisoWidgetPtr->ShowWidget();
	}
}

void UZeroUIComponent::InteractUIClose()
{
	if (ProvisoWidgetPtr && ProvisoWidgetPtr->IsInViewport())
	{
		ProvisoWidgetPtr->RemoveFromParent();
	}
}

void UZeroUIComponent::OperationInteract()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeUIOnly();
	}

	OperationWidgetPtr = CreateWidget<UZeroOperationWidget>(GetWorld(), OperationWidgetClass);
	if (OperationWidgetPtr)
	{
		OperationWidgetPtr->AddToViewport();
		FOnClickNextButton OnClickNextButton;
		OnClickNextButton.BindLambda([&]()
			{
				OperationNextButtonClick();
			});
		OperationWidgetPtr->SetDelegateClickNextButton(OnClickNextButton);
	}
}

void UZeroUIComponent::ProvisoInteract()
{
	UZeroGetProvisoWidget* GetProvisoWidgetInstance = CreateWidget<UZeroGetProvisoWidget>(GetWorld(), GetProvisoWidgetClass);
	if (GetProvisoWidgetInstance)
	{
		GetProvisoWidgetInstance->ShowWidget();
	}

	FZeroProvisoDataTable ProvisoData = UZeroSingleton::Get().GetProvisoData(ProvisoNum);

	if (!ProvisoData.ProvisoName.IsNone())
	{
		UZeroSingleton::Get().AddCollectedProviso(ProvisoData);

		GetProvisoWidgetInstance->SetProvisoInfo(ProvisoData.ProvisoName.ToString(), ProvisoData.Description);

		if (NoteWidgetPtr)
		{
			NoteWidgetPtr->SetNoteInfo(ProvisoData);
		}
	}



	ProvisoNum = 1;
}

void UZeroUIComponent::PauseMenuDisplay()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (UGameplayStatics::IsGamePaused(GetWorld()))
		return;
	UZeroPauseMenuWidget* PauseWidget = CreateWidget<UZeroPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PauseWidget)
	{
		PauseWidget->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		Interface->GetOwnerController()->InputModeUIOnly();
	}
}



