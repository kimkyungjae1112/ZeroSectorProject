// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroNoteWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Data/ZeroResearcherData.h"
#include "UI/ZeroProvisoButtonWidget.h"


void UZeroNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();

    ResearcherInfoBox->SetVisibility(ESlateVisibility::Hidden);

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UZeroNoteWidget::CloseClueDetail);
    }

}


void UZeroNoteWidget::ShowWidget()
{
    AddToViewport();
}

void UZeroNoteWidget::SetNoteInfo(const FZeroProvisoDataTable& ProvisoData)
{
    if (ProvisoWrapBox)
    {
        AddProvisoToUI(ProvisoData); 
    }
}

void UZeroNoteWidget::AddProvisoToUI(const FZeroProvisoDataTable& ProvisoData)
{
    if (!ProvisoButtonClass || !ProvisoWrapBox) return;

    UZeroProvisoButtonWidget* NewButtonWidget = CreateWidget<UZeroProvisoButtonWidget>(this, ProvisoButtonClass);
    if (!NewButtonWidget) return;

    NewButtonWidget->InitProviso(ProvisoData);
    NewButtonWidget->OnProvisoClicked.AddDynamic(this, &UZeroNoteWidget::ShowClueDetail);

    USizeBox* SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
    SizeBox->SetWidthOverride(100.f);
    SizeBox->SetHeightOverride(80.f);
    SizeBox->AddChild(NewButtonWidget);

    UWrapBoxSlot* WrapSlot = ProvisoWrapBox->AddChildToWrapBox(SizeBox);
    if (WrapSlot)
    {
        WrapSlot->SetPadding(FMargin(10.f, 10.f));
        WrapSlot->SetHorizontalAlignment(HAlign_Center);
        WrapSlot->SetVerticalAlignment(VAlign_Center);
    }
}

void UZeroNoteWidget::DisplayResearcher(UZeroResearcherData* ResearcherData)
{
    if (!ResearcherData) return;

    ResearcherInfoBox->SetVisibility(ESlateVisibility::Visible);

    if (PortraitImage)
        PortraitImage->SetBrushFromTexture(ResearcherData->Portrait);

    if (NameText)
        NameText->SetText(FText::FromString(ResearcherData->Name));

    if (AgeText)
        AgeText->SetText(FText::AsNumber(ResearcherData->Age));

    if (TrustText)
        TrustText->SetText(FText::AsNumber(ResearcherData->Trust));
}

void UZeroNoteWidget::ShowClueDetail(const FZeroProvisoDataTable& ProvisoData)
{
    if (DetailPopupBox)
    {
        DetailPopupBox->SetVisibility(ESlateVisibility::Visible);

        if (DetailNameText)
        {
            DetailNameText->SetText(FText::FromName(ProvisoData.ProvisoName));
            DetailNameText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
        }

        if (DetailDescText)
        {
            DetailDescText->SetText(FText::FromString(ProvisoData.Description));
            DetailDescText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
        }
    }
}

void UZeroNoteWidget::CloseClueDetail()
{
    if (DetailPopupBox)
    {
        DetailPopupBox->SetVisibility(ESlateVisibility::Hidden);
    }
}
