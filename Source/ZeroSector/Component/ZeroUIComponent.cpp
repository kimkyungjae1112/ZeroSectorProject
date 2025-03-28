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
	if (bIsNoteToggle)
	{
		NoteWidgetPtr->RemoveFromParent();
		bIsNoteToggle = false;
	}
	else
	{
		NoteWidgetPtr = CreateWidget<UZeroNoteWidget>(GetWorld(), NoteWidgetClass);
		NoteWidgetPtr->AddToViewport();
		bIsNoteToggle = true;
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
			NoteWidgetPtr->SetNoteInfo(ProvisoData.ProvisoName.ToString(), ProvisoData.Description);
		}
	}

	ProvisoNum = 1;
}



