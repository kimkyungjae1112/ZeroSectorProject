#include "UI/ZeroSelectResearcherWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZeroPlayerController.h"
#include "Data/ZeroSingleton.h"

void UZeroSelectResearcherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UZeroGameInstance>(GetGameInstance());

	ResearcherNames = { TEXT("Vaccine"), TEXT("Criminal"), TEXT("Normal1"), TEXT("Normal2"), TEXT("Normal3") };

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
		SelectedResearcherName = TEXT("");
	}
	else
	{
		SelectedResearcherIndex = Index;
		if (ResearcherNames.IsValidIndex(Index))
		{
			SelectedResearcherName = ResearcherNames[Index];
		}
		else
		{
			SelectedResearcherName = TEXT("");
		}
	}

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
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
			Buttons[i]->SetBackgroundColor(i == SelectedResearcherIndex ? FLinearColor::Green : FLinearColor::White);
		}
	}
}

void UZeroSelectResearcherWidget::OnClickSelectButton()
{
	if (!SelectedResearcherName.IsEmpty())
	{
		if (SelectCheckPopup && SelectCheckText)
		{
			FString Message = FString::Printf(TEXT("연구원 '%s'을(를) 선택하시겠습니까?"), *SelectedResearcherName);
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

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
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

	if (!SelectedResearcherName.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("Researcher '%s' is selected."), *SelectedResearcherName);

		UZeroSingleton::Get().SelectedResearcherName = SelectedResearcherName;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Select cancelled."));
	}

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->SelectSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->SelectSFX);
	}

	RemoveFromParent();
	OnCloseSelect.ExecuteIfBound();
}

void UZeroSelectResearcherWidget::OnClickSelectCancel()
{
	if (SelectCheckPopup)
	{
		SelectCheckPopup->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroSelectResearcherWidget::HideSelectWarningText()
{
	if (SelectWarningText)
	{
		SelectWarningText->SetVisibility(ESlateVisibility::Hidden);
	}
}
