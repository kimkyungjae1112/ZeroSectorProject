// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroUIComponent.h"
#include "ZeroHeader/ZeroUIHeader.h"
#include "Interface/ZeroUIComponentInterface.h"
#include "Player/ZeroPlayerController.h"
#include "Data/ZeroSingleton.h"
#include "ZeroSector.h"

UZeroUIComponent::UZeroUIComponent()
{

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
	if (Interface == nullptr) return;

	if (bIsNoteToggle)
	{
		NoteWidgetPtr->RemoveFromParent();
		bIsNoteToggle = false;
		Interface->GetOwnerController()->InputModeGameOnly();
		
	}
	else
	{
		NoteWidgetPtr = CreateWidget<UZeroNoteWidget>(GetWorld(), NoteWidgetClass);
		NoteWidgetPtr->AddToViewport();
		bIsNoteToggle = true;

		// 게임 + UI 모드로 전환하되, 키보드 포커스를 Note 위젯에 주지 않음
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(nullptr); // 또는 특정 포커스 위젯
		Interface->GetOwnerController()->SetInputMode(InputMode);
		Interface->GetOwnerController()->bShowMouseCursor = true;
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



