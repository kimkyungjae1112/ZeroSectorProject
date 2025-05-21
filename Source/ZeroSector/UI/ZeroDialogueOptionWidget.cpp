// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroDialogueOptionWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Component/ZeroDialogueComponent.h"

UZeroDialogueOptionWidget::UZeroDialogueOptionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroDialogueOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	DialogueOptionButton = Cast<UButton>(GetWidgetFromName(TEXT("DialogueOptionButton")));
	DialogueOptionTextBlock = Cast<UTextBlock>(GetWidgetFromName(TEXT("DialogueOptionText")));

	ensure(DialogueOptionButton);
	ensure(DialogueOptionTextBlock);

	DialogueOptionButton->OnClicked.AddDynamic(this, &UZeroDialogueOptionWidget::OnClickedOption);
}

void UZeroDialogueOptionWidget::SetDialogueComp(UZeroDialogueComponent* InDialogueComp)
{
	DialogueComp = InDialogueComp;
}

void UZeroDialogueOptionWidget::SetDialogueOptionText(const FText& InText)
{
	DialogueOptionTextBlock->SetText(InText);
}

void UZeroDialogueOptionWidget::OnClickedOption()
{
	DialogueComp->OnClickedOption(DialogueTableInOption, Reliability);

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());

	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->UIClickSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->UIClickSFX);
	}

}
