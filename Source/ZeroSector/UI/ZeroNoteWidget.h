// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ZeroProvisoDataTable.h"
#include "ZeroNoteWidget.generated.h"


struct FZeroProvisoDataTable;

class UImage;
class UTextBlock;
class UWrapBox;
class WrapBoxSlot;
class UVerticalBox;
class UButton;
class UZeroResearcherData;
class UCanvasPanel;

UCLASS()
class ZEROSECTOR_API UZeroNoteWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetNoteInfo(const FZeroProvisoDataTable& ProvisoData);

protected:
    virtual void NativeConstruct() override;

    UPROPERTY(meta = (BindWidget))
    UWrapBox* ProvisoWrapBox;

    int32 CurrentProvisoCount = 0;

    void ShowWidget();

    void AddProvisoToUI(const FZeroProvisoDataTable& ProvisoData);

    // 연구원
public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    void DisplayResearcher(UZeroResearcherData* ResearcherData);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Researchers")
    UZeroResearcherData* Researcher1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Researchers")
    UZeroResearcherData* Researcher2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Researchers")
    UZeroResearcherData* Researcher3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Researchers")
    UZeroResearcherData* Researcher4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Researchers")
    UZeroResearcherData* Researcher5;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PortraitImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> NameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> AgeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> TrustText;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ResearcherInfoBox;

    // 단서
protected:

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* DetailPopupBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DetailNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* DetailDescText;

    UPROPERTY(meta = (BindWidget))
    UButton* CloseButton;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class UZeroProvisoButtonWidget> ProvisoButtonClass;

    UFUNCTION()
    void ShowClueDetail(const FZeroProvisoDataTable& ProvisoData);

    FZeroProvisoDataTable LastClickedProvisoData;

    UFUNCTION()
    void CloseClueDetail();

};