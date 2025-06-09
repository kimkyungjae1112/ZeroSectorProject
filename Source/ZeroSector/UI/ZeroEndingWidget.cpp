// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroEndingWidget.h"
#include "Components/Image.h"
#include "Player/ZeroPlayerController.h"
#include "Kismet/GameplayStatics.h"

UZeroEndingWidget::UZeroEndingWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	HappyEndingImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/ending_happy.ending_happy'"));
	NormalEndingImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/ending_normal.ending_normal'"));
	BadEndingImage = LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Blueprints/UI/Image/ending_bad.ending_bad'"));

	FadeInEvent.BindDynamic(this, &UZeroEndingWidget::FadeInPlay);
	RemoveEvent.BindDynamic(this, &UZeroEndingWidget::RemoveWidget);
}

void UZeroEndingWidget::HappyEndingDisplay() const
{
	EndingImage->SetBrushFromTexture(HappyEndingImage);
}

void UZeroEndingWidget::NormalEndingDisplay() const
{
	EndingImage->SetBrushFromTexture(NormalEndingImage);
}

void UZeroEndingWidget::BadEndingDisplay() const
{
	EndingImage->SetBrushFromTexture(BadEndingImage);
}

void UZeroEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EndingImage = Cast<UImage>(GetWidgetFromName(TEXT("EndingImage")));
	ensure(EndingImage);
}

void UZeroEndingWidget::FadeOutPlay()
{
	PlayAnimation(FadeOut);

	BindToAnimationFinished(FadeOut, FadeInEvent);
}

void UZeroEndingWidget::FadeInPlay()
{
	PlayAnimation(FadeIn);

	BindToAnimationFinished(FadeIn, RemoveEvent);
}

void UZeroEndingWidget::RemoveWidget()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenuMap"));
	}
}
