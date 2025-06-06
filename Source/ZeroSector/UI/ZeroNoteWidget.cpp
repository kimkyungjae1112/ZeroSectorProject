// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroNoteWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/CanvasPanel.h"
#include "Data/ZeroResearcherData.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ZeroProvisoButtonWidget.h"
#include "Player/ZeroPlayerController.h"
#include "UI/ZeroAfternoonHUDWidget.h"
#include "Game/ZeroGameModeBase.h"
#include "Data/ZeroSingleton.h"
#include "Data/ZeroProvisoDataTable.h"


void UZeroNoteWidget::NativeConstruct()
{
	Super::NativeConstruct();

    GI = Cast<UZeroGameInstance>(GetGameInstance());

    ResearcherInfoBox->SetVisibility(ESlateVisibility::Hidden);

    if (CloseButton && !CloseButton->OnClicked.IsAlreadyBound(this, &UZeroNoteWidget::CloseClueDetail))
    {
        CloseButton->OnClicked.AddDynamic(this, &UZeroNoteWidget::CloseClueDetail);
    }

    if (InterviewButton && !InterviewButton->OnClicked.IsAlreadyBound(this, &UZeroNoteWidget::OnInterviewButtonClicked))
    {
        InterviewButton->OnClicked.AddDynamic(this, &UZeroNoteWidget::OnInterviewButtonClicked);
    }

}


void UZeroNoteWidget::ShowWidget()
{
    if (DayText)
    {
        int32 Day = AZeroGameModeBase::Day;

        FString DayString = FString::Printf(TEXT("Day %d"), Day);
        DayText->SetText(FText::FromString(DayString));
    }

    if (CurrentNoteResearcher)
    {
        ShowResearcherProviso(CurrentNoteResearcher);
    }
}


void UZeroNoteWidget::DisplayResearcher(UZeroResearcherData* ResearcherData)
{
    if (!ResearcherData) return;

    CurrentInterviewResearcher = ResearcherData;
    CurrentNoteResearcher = ResearcherData;

    ResearcherInfoBox->SetVisibility(ESlateVisibility::Visible);

    if (PortraitImage)
        PortraitImage->SetBrushFromTexture(ResearcherData->Portrait);

    if (NameText)
        NameText->SetText(FText::FromString(ResearcherData->Name));

    if (AgeText)
        AgeText->SetText(FText::AsNumber(ResearcherData->Age));

    if (RelLevelImage)
        RelLevelImage->SetBrushFromTexture(ResearcherData->GetTrustLevelImage());

    PlayUIClickSound();
}

void UZeroNoteWidget::OnInterviewButtonClicked()
{
    if (CurrentInterviewResearcher)
    {
        AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetOwningPlayer());
        if (PC && PC->GetAfternoonHUDWidget())
        {
            PC->GetAfternoonHUDWidget()->ShowInterviewText(CurrentInterviewResearcher->Name);
            PC->SelectedInterviewName = CurrentInterviewResearcher->Name; 
        }
    }

    PlayUIClickSound();
}

void UZeroNoteWidget::ShowClueDetail(const FZeroProvisoDataTable& ProvisoData)
{
    if (DetailPopupBox)
    {
        DetailPopupBox->SetVisibility(ESlateVisibility::Visible);

        if (DetailNameText)
        {
            DetailNameText->SetText(FText::FromName(ProvisoData.ProvisoName));
            DetailNameText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
        }

        if (DetailDescText)
        {
            DetailDescText->SetText(FText::FromString(ProvisoData.Description));
            DetailDescText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
        }

        if (DetailImage)
        {
            UTexture2D* Image = ProvisoData.ProvisoImage.LoadSynchronous();
            if (Image)
            {
                DetailImage->SetBrushFromTexture(Image);
            }
            else
            {
                DetailImage->SetBrushFromTexture(nullptr);
            }
        }
    }

    PlayUIClickSound();
}

void UZeroNoteWidget::CloseClueDetail()
{
    if (DetailPopupBox)
    {
        DetailPopupBox->SetVisibility(ESlateVisibility::Hidden);
    }

    PlayUIClickSound();
}

void UZeroNoteWidget::PlayUIClickSound()
{
    if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
    {
        UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
    }
}

void UZeroNoteWidget::ShowResearcherProviso(UZeroResearcherData* ResearcherData)
{
    if (!ResearcherData || !ProvisoWrapBox)
    {
        return;
    }

    ProvisoWrapBox->ClearChildren();

    const TArray<FZeroProvisoDataTable>& AllProvisos = UZeroSingleton::Get().GetAllProvisoData();
    const TArray<FZeroProvisoDataTable>& Collected = UZeroSingleton::Get().GetCollectedProvisos();

    for (int32 i = 0; i < ResearcherData->ProvisoNum; ++i)
    {
        FName RowName = *FString::FromInt(ResearcherData->ProvisoStart + i);
        const FZeroProvisoDataTable& Proviso = UZeroSingleton::Get().GetProvisoData(RowName);

        UZeroProvisoButtonWidget* NewButton = CreateWidget<UZeroProvisoButtonWidget>(this, ProvisoButtonClass);
        if (!NewButton)
        {
            continue;
        }

        NewButton->InitProviso(Proviso);
        NewButton->OnProvisoClicked.AddDynamic(this, &UZeroNoteWidget::ShowClueDetail);

        bool bIsCollected = Collected.ContainsByPredicate(
            [&](const FZeroProvisoDataTable& C) { return C.ProvisoName == Proviso.ProvisoName; });

        NewButton->SetLocked(!bIsCollected);

        USizeBox* Box = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
        Box->SetWidthOverride(450.f);
        Box->SetHeightOverride(80.f);
        Box->AddChild(NewButton);

        UVerticalBoxSlot* BoxSlot = ProvisoWrapBox->AddChildToVerticalBox(Box);
        if (BoxSlot)
        {
            BoxSlot->SetPadding(FMargin(5.f, 5.f));
            BoxSlot->SetHorizontalAlignment(HAlign_Center);
        }
    }
}



void UZeroNoteWidget::ClearResearcherInfo()
{
    if (ResearcherInfoBox)
    {
        ResearcherInfoBox->SetVisibility(ESlateVisibility::Hidden);
    }

    if (ProvisoWrapBox)
    {
        ProvisoWrapBox->ClearChildren(); 
    }
}

void UZeroNoteWidget::SetCurrentNoteResearcher(UZeroResearcherData* Researcher)
{
    CurrentNoteResearcher = Researcher;
}




