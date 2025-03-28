// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroGetProvisoWidget.generated.h"


/**
 * 
 */
UCLASS()
class ZEROSECTOR_API UZeroGetProvisoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(Category = "UI")
    void ShowWidget();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ProvisoNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* ProvisoDescriptionText;

	// 단서 정보를 위젯에 전달하는 함수
	void SetProvisoInfo(const FString& ProvisoName, const FString& ProvisoDescription);

protected:
	virtual void NativeConstruct() override;


private:
    FTimerHandle WidgetTimerHandle;
};

