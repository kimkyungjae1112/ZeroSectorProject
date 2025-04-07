// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroExcludeResearcherWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Player/ZeroPlayerController.h"

void UZeroExcludeResearcherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExRes1Button) ExRes1Button->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickResearcher1);
	if (ExRes2Button) ExRes2Button->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickResearcher2);
	if (ExRes3Button) ExRes3Button->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickResearcher3);
	if (ExRes4Button) ExRes4Button->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickResearcher4);
	if (ExRes5Button) ExRes5Button->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickResearcher5);

	if (ExcludeButton) ExcludeButton->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickExcludeButton);
	if (NotExcludeButton) NotExcludeButton->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickNotExcludeButton);

	if (ExcludeOKButton) ExcludeOKButton->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickExcludeOK);
	if (ExcludeCancelButton) ExcludeCancelButton->OnClicked.AddDynamic(this, &UZeroExcludeResearcherWidget::OnClickExcludeCancel);

	if (ExcludeCheckPopup)
	{
		ExcludeCheckPopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ExcludeWarningText)
	{
		ExcludeWarningText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UZeroExcludeResearcherWidget::OnClickResearcher1() { HandleResearcherSelection(0); }
void UZeroExcludeResearcherWidget::OnClickResearcher2() { HandleResearcherSelection(1); }
void UZeroExcludeResearcherWidget::OnClickResearcher3() { HandleResearcherSelection(2); }
void UZeroExcludeResearcherWidget::OnClickResearcher4() { HandleResearcherSelection(3); }
void UZeroExcludeResearcherWidget::OnClickResearcher5() { HandleResearcherSelection(4); }

void UZeroExcludeResearcherWidget::HandleResearcherSelection(int32 Index)
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

void UZeroExcludeResearcherWidget::UpdateButtonStyles()
{
	TArray<UButton*> Buttons = { ExRes1Button, ExRes2Button, ExRes3Button, ExRes4Button, ExRes5Button };

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

void UZeroExcludeResearcherWidget::OnClickExcludeButton()
{
	if (SelectedResearcherIndex >= 0)
	{
		if (ExcludeCheckPopup && ExcludeCheckText)
		{
			FString Message = FString::Printf(TEXT("연구원 %d을(를) 배제하시겠습니까?"), SelectedResearcherIndex + 1);
			ExcludeCheckText->SetText(FText::FromString(Message));
			ExcludeCheckPopup->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (ExcludeWarningText)
		{
			ExcludeWarningText->SetVisibility(ESlateVisibility::Visible);

			GetWorld()->GetTimerManager().ClearTimer(WarningTextTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(
				WarningTextTimerHandle,
				this,
				&UZeroExcludeResearcherWidget::HideExcludeWarningText,
				2.0f,
				false
			);
		}
	}
}

void UZeroExcludeResearcherWidget::OnClickNotExcludeButton()
{
	if (ExcludeCheckPopup && ExcludeCheckText)
	{
		ExcludeCheckText->SetText(FText::FromString(TEXT("배제하지 않으시겠습니까?")));
		ExcludeCheckPopup->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZeroExcludeResearcherWidget::OnClickExcludeOK()
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
		UE_LOG(LogTemp, Log, TEXT("Researcher %d is excluded."), SelectedResearcherIndex + 1);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Exclude cancelled."));
	}

	RemoveFromParent();
	OnCloseExclude.ExecuteIfBound();
}

void UZeroExcludeResearcherWidget::OnClickExcludeCancel()
{
	if (ExcludeCheckPopup)
	{
		ExcludeCheckPopup->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UZeroExcludeResearcherWidget::HideExcludeWarningText()
{
	if (ExcludeWarningText)
	{
		ExcludeWarningText->SetVisibility(ESlateVisibility::Hidden);
	}
}
