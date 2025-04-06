// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroDialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "ZeroSector.h"

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

	ZE_LOG(LogZeroSector, Display, TEXT("너는 왜 생성이 됐다가 안되니 ㅆ,,,"));
}

UScrollBox* UZeroDialogueWidget::GetScrollBox() const
{
	return OptionScrollBox;
}

void UZeroDialogueWidget::SetDialogueText(const FText& InSpeaker, const FText& InDialogue)
{
	ZE_LOG(LogZeroSector, Display, TEXT("말하는 애 : %s"), *InSpeaker.ToString());
	ZE_LOG(LogZeroSector, Display, TEXT("대화 내용 : %s"), *InDialogue.ToString());
	SpeakerTextBlock->SetText(InSpeaker);
	DialogueTextBlock->SetText(InDialogue);
}

void UZeroDialogueWidget::UpdateDialogue(const FText& InDialogue)
{
	DialogueTextBlock->SetText(InDialogue);
}
