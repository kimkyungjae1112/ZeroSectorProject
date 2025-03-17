// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroProvisoWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZEROSECTOR_API UZeroProvisoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    UFUNCTION(Category = "UI")
    void ShowWidget();

	
};



