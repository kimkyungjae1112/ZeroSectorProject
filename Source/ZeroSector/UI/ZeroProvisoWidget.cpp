// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroProvisoWidget.h"

void UZeroProvisoWidget::ShowWidget()
{
    if (!IsInViewport()) 
    {
        AddToViewport();
    }
}
