// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroGetProvisoWidget.h"

void UZeroGetProvisoWidget::ShowWidget()
{
    AddToViewport();


    GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &UZeroGetProvisoWidget::HideWidget, 3.0f, false);
}

void UZeroGetProvisoWidget::HideWidget()
{
    RemoveFromParent();
}
