// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Delegates/DelegateCombinations.h"
#include "ZeroProvisoButtonWidget.generated.h"

class UButton;
class UTextBlock;

/** 단서 버튼 클릭 델리게이트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProvisoButtonClicked, const FZeroProvisoDataTable&, Data);

UCLASS()
class ZEROSECTOR_API UZeroProvisoButtonWidget : public UUserWidget
{
    GENERATED_BODY()

    public:
    void InitProviso(const FZeroProvisoDataTable& InProvisoData);

    UPROPERTY(BlueprintAssignable)
    FOnProvisoButtonClicked OnProvisoClicked;

    const FZeroProvisoDataTable& GetProvisoData() const { return ProvisoData; }

protected:
    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void HandleClick();

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ProvisoButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ProvisoText;

    FZeroProvisoDataTable ProvisoData;
};
