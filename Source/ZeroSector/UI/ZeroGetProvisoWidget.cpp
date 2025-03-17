// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroGetProvisoWidget.h"

void UZeroGetProvisoWidget::ShowWidget()
{
    AddToViewport();
    GetWorld()->GetTimerManager().SetTimer(WidgetTimerHandle, this, &UZeroGetProvisoWidget::RemoveFromParent, 3.0f, false);
}


