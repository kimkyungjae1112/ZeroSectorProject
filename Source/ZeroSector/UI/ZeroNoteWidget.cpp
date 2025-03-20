// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroNoteWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Data/ZeroProvisoDataTable.h"

void UZeroNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();
}



void UZeroNoteWidget::ShowWidget()
{
    AddToViewport();
}

void UZeroNoteWidget::SetNoteInfo(const FString& ProvisoName, const FString& Description)
{

    if (ProvisoListBox)
    {
        AddProvisoToUI(ProvisoName, Description);
    }
}

void UZeroNoteWidget::AddProvisoToUI(const FString& ProvisoName, const FString& Description)
{
    UTextBlock* NewTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

    if (NewTextBlock)
    {
        NewTextBlock->SetText(FText::FromString(ProvisoName + TEXT(": ") + Description));

        FSlateColor BlackColor = FSlateColor(FLinearColor::Black);
        NewTextBlock->SetColorAndOpacity(BlackColor);

        ProvisoListBox->AddChild(NewTextBlock);
    }
}