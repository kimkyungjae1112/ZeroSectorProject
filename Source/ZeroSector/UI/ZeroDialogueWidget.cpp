// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroDialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

UZeroDialogueWidget::UZeroDialogueWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SpeakerTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Speaker")));
	DialogueTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("Dialogue")));
	OptionScrollBox = Cast<UScrollBox>(GetWidgetFromName(TEXT("OptionScrollBox")));

	ensure(SpeakerTextBlock);
	ensure(DialogueTextBlock);
	ensure(OptionScrollBox);
}

UScrollBox* UZeroDialogueWidget::GetScrollBox() const
{
	return OptionScrollBox;
}

void UZeroDialogueWidget::SetDialogueText(const FText& InSpeaker, const FText& InDialogue)
{
	SpeakerTextBlock->SetText(InSpeaker);
	DialogueTextBlock->SetText(InDialogue);
}

void UZeroDialogueWidget::UpdateDialogue(const FText& InDialogue)
{
	DialogueTextBlock->SetText(InDialogue);
}
