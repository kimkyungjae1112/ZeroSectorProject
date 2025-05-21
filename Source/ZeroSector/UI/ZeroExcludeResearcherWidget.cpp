#include "UI/ZeroExcludeResearcherWidget.h"
#include "Data/ZeroSingleton.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ZeroPlayerController.h"

void UZeroExcludeResearcherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GI = Cast<UZeroGameInstance>(GetGameInstance());

	ResearcherNames = { TEXT("Vaccine"), TEXT("Criminal"), TEXT("Normal1"), TEXT("Normal2"), TEXT("Normal3") };

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
	if (ExcludedResearcherIndex == Index)
	{
		ExcludedResearcherIndex = -1;
		ExcludedResearcherName = TEXT("");
	}
	else
	{
		ExcludedResearcherIndex = Index;

		// 선택한 연구원 이름 저장
		if (ResearcherNames.IsValidIndex(Index))
		{
			ExcludedResearcherName = ResearcherNames[Index];
		}
		else
		{
			ExcludedResearcherName = TEXT("");
		}
	}

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
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
			Buttons[i]->SetBackgroundColor(i == ExcludedResearcherIndex ? FLinearColor::Green : FLinearColor::White);
		}
	}
}

void UZeroExcludeResearcherWidget::OnClickExcludeButton()
{
	if (!ExcludedResearcherName.IsEmpty())
	{
		if (ExcludeCheckPopup && ExcludeCheckText)
		{
			FString Message = FString::Printf(TEXT("연구원 '%s'을(를) 배제하시겠습니까?"), *ExcludedResearcherName);
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

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}


void UZeroExcludeResearcherWidget::OnClickNotExcludeButton()
{
	if (ExcludeCheckPopup && ExcludeCheckText)
	{
		ExcludeCheckText->SetText(FText::FromString(TEXT("배제하지 않으시겠습니까?")));
		ExcludeCheckPopup->SetVisibility(ESlateVisibility::Visible);
	}

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroExcludeResearcherWidget::OnClickExcludeOK()
{
	if (!ExcludedResearcherName.IsEmpty())
	{
		UZeroSingleton::Get().ExcludedResearcherName = ExcludedResearcherName;
		UE_LOG(LogTemp, Log, TEXT("Excluded researcher saved to singleton: %s"), *ExcludedResearcherName);
	}

	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AZeroPlayerController* ZeroPC = Cast<AZeroPlayerController>(PC))
		{
			ZeroPC->RemoveBlurEffect();
			ZeroPC->InputModeGameOnly();
		}
	}

	if (ExcludedResearcherIndex >= 0 && ResearcherNames.IsValidIndex(ExcludedResearcherIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Researcher '%s' is excluded."), *ResearcherNames[ExcludedResearcherIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Exclude cancelled."));
	}

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ExcludeSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->ExcludeSFX);
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

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroExcludeResearcherWidget::HideExcludeWarningText()
{
	if (ExcludeWarningText)
	{
		ExcludeWarningText->SetVisibility(ESlateVisibility::Hidden);
	}
}
