// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroGetProvisoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UZeroGetProvisoWidget::NativeConstruct()
{
	Super::NativeConstruct();
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

void UZeroGetProvisoWidget::ShowWidget()
{
    AddToViewport();
    GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &UZeroGetProvisoWidget::RemoveFromParent, 3.0f, false);
}



