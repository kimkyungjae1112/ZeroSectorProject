// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroDialogueWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Component/ZeroDialogueComponent.h"
#include "Component/Input/ZeroInputAfternoonComponent.h"
#include "Game/ZeroSoundManager.h"
#include "Game/ZeroGameInstance.h"
#include "Kismet/GameplayStatics.h"
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
	NextDialogueButton = Cast<UButton>(GetWidgetFromName(TEXT("NextDialogueButton")));

	ensure(SpeakerTextBlock);
	ensure(DialogueTextBlock);
	ensure(OptionScrollBox);
	ensure(NextDialogueButton);

	NextDialogueButton->OnClicked.AddDynamic(this, &UZeroDialogueWidget::NextDialogueButtonClicked);
}

UScrollBox* UZeroDialogueWidget::GetScrollBox() const
{
	return OptionScrollBox;
}

void UZeroDialogueWidget::SetDialogueText(const FText& InSpeaker, const FText& InDialogue)
{
	/*ZE_LOG(LogZeroSector, Display, TEXT("말하는 애 : %s"), *InSpeaker.ToString());
	ZE_LOG(LogZeroSector, Display, TEXT("대화 내용 : %s"), *InDialogue.ToString());*/
	SpeakerTextBlock->SetText(InSpeaker);
	DialogueTextBlock->SetText(InDialogue);
}

void UZeroDialogueWidget::UpdateDialogue(const FText& InDialogue)
{
	DialogueTextBlock->SetText(InDialogue);
}

void UZeroDialogueWidget::NextDialogueButtonClicked()
{
	if (UZeroInputAfternoonComponent::CurrentDialogueNPC)
	{
		UZeroDialogueComponent* DialogueComp = Cast<UZeroDialogueComponent>(UZeroInputAfternoonComponent::CurrentDialogueNPC->GetComponentByClass<UZeroDialogueComponent>());
		DialogueComp->StartDialogue();
	}

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}
}

void UZeroDialogueWidget::DisplayNextDialogueButton()
{
	NextDialogueButton->SetVisibility(ESlateVisibility::Visible);
}

void UZeroDialogueWidget::CloseNextDialogueButton()
{
	NextDialogueButton->SetVisibility(ESlateVisibility::Hidden);
}
