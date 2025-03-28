// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ZeroProvisoDataTable.h"
#include "ZeroProvisoButtonWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroProvisoButtonWidget : public UUserWidget
{
    GENERATED_BODY()

    public:
    void InitProviso(const FZeroProvisoDataTable& InProvisoData);

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* ProvisoButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* ProvisoText;

    FZeroProvisoDataTable StoredData;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProvisoButtonClicked, const FZeroProvisoDataTable&, Data);
    UPROPERTY(BlueprintAssignable)
    FOnProvisoButtonClicked OnProvisoClicked;

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void HandleClick();
};