// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroEnforceBoardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

UZeroEnforceBoardWidget::UZeroEnforceBoardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroEnforceBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PistolLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("PistolLevel")));
	RifleLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("RifleLevel")));
	ShotgunLevelText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ShotgunLevel")));

	PistolEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("PistolEnfoButton")));
	RifleEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("RifleEnfoButton")));
	ShotgunEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("ShotgunEnfoButton")));
	CancelEnfoButton = Cast<UButton>(GetWidgetFromName(TEXT("CancelButton")));

	ensure(PistolLevelText);
	ensure(RifleLevelText);
	ensure(ShotgunLevelText);

	ensure(PistolEnfoButton);
	ensure(RifleEnfoButton);
	ensure(ShotgunEnfoButton);
	ensure(CancelEnfoButton);

	PistolEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::PistolEnfoButtonClicked);
	RifleEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::RifleEnfoButtonClicked);
	ShotgunEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::ShotgunEnfoButtonClicked);
	CancelEnfoButton->OnClicked.AddDynamic(this, &UZeroEnforceBoardWidget::CloseEnfoButtonClicked);
}

void UZeroEnforceBoardWidget::PistolEnfoButtonClicked()
{
	// 어떻게 무기의 현재 레벨을 UI로 띄우고, 무기에 전달할지 구현해야함
		// 현재까지는 무기 스텟을 DataTable로 바꾸고
		// 무기 강화 UI 를 띄우는 것까지 구현 완료
}

void UZeroEnforceBoardWidget::RifleEnfoButtonClicked()
{
}

void UZeroEnforceBoardWidget::ShotgunEnfoButtonClicked()
{
}

void UZeroEnforceBoardWidget::CloseEnfoButtonClicked()
{
	OnCancelButton.ExecuteIfBound();
}
