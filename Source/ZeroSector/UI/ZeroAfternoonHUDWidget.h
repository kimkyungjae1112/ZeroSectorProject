// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroAfternoonHUDWidget.generated.h"

class UProgressBar;

UCLASS()
class ZEROSECTOR_API UZeroAfternoonHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroAfternoonHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProgressBar> ActivePoint;
};
