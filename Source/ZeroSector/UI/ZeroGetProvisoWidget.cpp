// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroGetProvisoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Interface/ZeroUIComponentInterface.h"

void UZeroGetProvisoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WriteButton)
	{
		WriteButton->OnClicked.AddDynamic(this, &UZeroGetProvisoWidget::OnWriteClicked);
	}

	if (ThrowButton)
	{
		ThrowButton->OnClicked.AddDynamic(this, &UZeroGetProvisoWidget::OnThrowClicked);
	}
}

void UZeroGetProvisoWidget::OnWriteClicked()
{
	OnProvisoConfirmed.Broadcast(CurrentProvisoData);
	RemoveFromParent();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

void UZeroGetProvisoWidget::OnThrowClicked()
{
	OnProvisoRejected.Broadcast();
	RemoveFromParent();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

void UZeroGetProvisoWidget::SetProvisoInfo(const FString& ProvisoName, const FString& ProvisoDescription)
{
	if (ProvisoNameText)
	{
		ProvisoNameText->SetText(FText::FromString(ProvisoName));
	}

	if (ProvisoDescriptionText)
	{
		ProvisoDescriptionText->SetText(FText::FromString(ProvisoDescription));
	}
}

void UZeroGetProvisoWidget::SetProvisoImage(UTexture2D* InImage)
{
	if (ProvisoImage && InImage)
	{
		ProvisoImage->SetBrushFromTexture(InImage);
	}
}

void UZeroGetProvisoWidget::SetCurrentProvisoData(const FZeroProvisoDataTable& InData)
{
	CurrentProvisoData = InData;
}

void UZeroGetProvisoWidget::ShowWidget()
{
    AddToViewport();
}



