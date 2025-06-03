// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroProvisoButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UZeroProvisoButtonWidget::SetLocked(bool bLocked)
{
    if (LockImage)
    {
        LockImage->SetVisibility(bLocked ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    if (ProvisoButton)
    {
        ProvisoButton->SetIsEnabled(!bLocked);
    }

}

void UZeroProvisoButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ProvisoButton && !ProvisoButton->OnClicked.IsAlreadyBound(this, &UZeroProvisoButtonWidget::HandleClick))
    {
        ProvisoButton->OnClicked.AddDynamic(this, &UZeroProvisoButtonWidget::HandleClick);
    }
}

void UZeroProvisoButtonWidget::InitProviso(const FZeroProvisoDataTable& InProvisoData)
{
    ProvisoData = InProvisoData;

    if (ProvisoText)
    {
        ProvisoText->SetText(FText::FromName(InProvisoData.ProvisoName));
    }
}

void UZeroProvisoButtonWidget::HandleClick()
{
    OnProvisoClicked.Broadcast(ProvisoData);
}