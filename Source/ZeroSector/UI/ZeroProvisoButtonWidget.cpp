// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroProvisoButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UZeroProvisoButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ProvisoButton)
    {
        ProvisoButton->OnClicked.AddDynamic(this, &UZeroProvisoButtonWidget::HandleClick);
    }
}

void UZeroProvisoButtonWidget::InitProviso(const FZeroProvisoDataTable& InProvisoData)
{
    StoredData = InProvisoData;

    if (ProvisoText)
    {
        ProvisoText->SetText(FText::FromName(InProvisoData.ProvisoName));
    }
}

void UZeroProvisoButtonWidget::HandleClick()
{
    OnProvisoClicked.Broadcast(StoredData);
}
