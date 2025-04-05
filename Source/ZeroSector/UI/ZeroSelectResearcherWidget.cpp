// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroSelectResearcherWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Player/ZeroPlayerController.h"

void UZeroSelectResearcherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelRes1Button) SelRes1Button->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickResearcher1);
	if (SelRes2Button) SelRes2Button->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickResearcher2);
	if (SelRes3Button) SelRes3Button->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickResearcher3);
	if (SelRes4Button) SelRes4Button->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickResearcher4);
	if (SelRes5Button) SelRes5Button->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickResearcher5);

	if (SelectButton) SelectButton->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickSelectButton);

	if (SelectOKButton) SelectOKButton->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickSelectOK);
	if (SelectCancelButton) SelectCancelButton->OnClicked.AddDynamic(this, &UZeroSelectResearcherWidget::OnClickSelectCancel);

	if (SelectCheckPopup)
	{
		SelectCheckPopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (SelectWarningText)
	{
		SelectWarningText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UZeroSelectResearcherWidget::OnClickResearcher1() { HandleResearcherSelection(0); }
void UZeroSelectResearcherWidget::OnClickResearcher2() { HandleResearcherSelection(1); }
void UZeroSelectResearcherWidget::OnClickResearcher3() { HandleResearcherSelection(2); }
void UZeroSelectResearcherWidget::OnClickResearcher4() { HandleResearcherSelection(3); }
void UZeroSelectResearcherWidget::OnClickResearcher5() { HandleResearcherSelection(4); }

void UZeroSelectResearcherWidget::HandleResearcherSelection(int32 Index)
{
	if (SelectedResearcherIndex == Index)
	{
		SelectedResearcherIndex = -1;
	}
	else
	{
		SelectedResearcherIndex = Index;
	}

	UpdateButtonStyles();
}

void UZeroSelectResearcherWidget::UpdateButtonStyles()
{
	TArray<UButton*> Buttons = { SelRes1Button, SelRes2Button, SelRes3Button, SelRes4Button, SelRes5Button };

	for (int32 i = 0; i < Buttons.Num(); ++i)
	{
		if (Buttons[i])
		{
			if (i == SelectedResearcherIndex)
			{
				Buttons[i]->SetBackgroundColor(FLinearColor::Green);
			}
			else
			{
				Buttons[i]->SetBackgroundColor(FLinearColor::White);
			}
		}
	}
}

void UZeroSelectResearcherWidget::OnClickSelectButton()
{
	if (SelectedResearcherIndex >= 0)
	{
		if (SelectCheckPopup && SelectCheckText)
		{
			FString Message = FString::Printf(TEXT("연구원 %d을(를) 선택하시겠습니까?"), SelectedResearcherIndex + 1);
			SelectCheckText->SetText(FText::FromString(Message));
			SelectCheckPopup->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (SelectWarningText)
		{
			SelectWarningText->SetVisibility(ESlateVisibility::Visible);

			GetWorld()->GetTimerManager().ClearTimer(WarningTextTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(
				WarningTextTimerHandle,
				this,
				&UZeroSelectResearcherWidget::HideSelectWarningText,
				2.0f,
				false
			);
		}
	}
}


void UZeroSelectResearcherWidget::OnClickSelectOK()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AZeroPlayerController* ZeroPC = Cast<AZeroPlayerController>(PC))
		{
			ZeroPC->RemoveBlurEffect();
			ZeroPC->InputModeGameOnly();
		}
	}

	if (SelectedResearcherIndex >= 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Researcher %d is selected."), SelectedResearcherIndex + 1);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Select cancelled."));
	}

	RemoveFromParent(); 
}

void UZeroSelectResearcherWidget::OnClickSelectCancel()
{
	if (SelectCheckPopup)
	{
		SelectCheckPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UZeroSelectResearcherWidget::HideSelectWarningText()
{
	if (SelectWarningText)
	{
		SelectWarningText->SetVisibility(ESlateVisibility::Hidden);
	}
}