// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroAfternoonHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ZeroHUDInterface.h"
#include "Components/TextBlock.h"
#include "ZeroSector.h"

UZeroAfternoonHUDWidget::UZeroAfternoonHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxActivePoint = -1.f;
}

void UZeroAfternoonHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ActivePointBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ActivePoint")));

	ensure(ActivePointBar);

	IZeroHUDInterface* Interface = Cast<IZeroHUDInterface>(GetOwningPlayerPawn());
	if (Interface)	Interface->SetAfternoonHUDWidget(this);
}

void UZeroAfternoonHUDWidget::SetMaxActivePoint(float InMaxActivePoint)
{
	MaxActivePoint = InMaxActivePoint;
}

void UZeroAfternoonHUDWidget::UpdateAPBar(float InActivePoint)
{
	ActivePointBar->SetPercent(InActivePoint / MaxActivePoint);
	ZE_LOG(LogZeroSector, Display, TEXT("AP : %f"), InActivePoint);
}

void UZeroAfternoonHUDWidget::ShowInterviewText(const FString& ResearcherName)
{
	if (InterviewText)
	{
		FString DisplayText = FString::Printf(TEXT("Interview Target: %s"), *ResearcherName);
		InterviewText->SetText(FText::FromString(DisplayText));
		InterviewText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UZeroAfternoonHUDWidget::HideInterviewText()
{
	if (InterviewText)
	{
		InterviewText->SetVisibility(ESlateVisibility::Hidden);
	}
}