// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroCrossHairWidget.h"
#include "Components/Image.h"

UZeroCrossHairWidget::UZeroCrossHairWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CrosshairSpread = 0.0f;
    CrosshairRecoverySpeed = 5.0f;    
    MaxCrosshairSpread = 50.f;
}

void UZeroCrossHairWidget::NativeConstruct()
{
	Super::NativeConstruct();

    CrosshairLeft = Cast<UImage>(GetWidgetFromName(TEXT("CrosshairLeft")));
    CrosshairRight = Cast<UImage>(GetWidgetFromName(TEXT("CrosshairRight")));
    CrosshairTop = Cast<UImage>(GetWidgetFromName(TEXT("CrosshairTop")));
    CrosshairBottom = Cast<UImage>(GetWidgetFromName(TEXT("CrosshairBottom")));
}

void UZeroCrossHairWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // 시간이 지남에 따라 스프레드 값을 0으로 보간
    if (CrosshairSpread > 0.0f)
    {
        CrosshairSpread = FMath::FInterpTo(CrosshairSpread, 0.0f, InDeltaTime, CrosshairRecoverySpeed);
    }

    // 각 크로스 헤어 이미지의 위치를 스프레드 값에 따라 조정 (중심 기준)
    if (CrosshairLeft)
    {
        CrosshairLeft->SetRenderTranslation(FVector2D(-CrosshairSpread, 0));
    }
    if (CrosshairRight)
    {
        CrosshairRight->SetRenderTranslation(FVector2D(CrosshairSpread, 0));
    }
    if (CrosshairTop)
    {
        CrosshairTop->SetRenderTranslation(FVector2D(0, -CrosshairSpread));
    }
    if (CrosshairBottom)
    {
        CrosshairBottom->SetRenderTranslation(FVector2D(0, CrosshairSpread));
    }
}

void UZeroCrossHairWidget::IncreaseSpread(float Amount)
{
    CrosshairSpread = FMath::Clamp(CrosshairSpread + Amount, 0.0f, MaxCrosshairSpread);
}

void UZeroCrossHairWidget::HitCrosshair()
{
    CrosshairLeft->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
    CrosshairRight->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
    CrosshairTop->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
    CrosshairBottom->SetColorAndOpacity(FLinearColor(1.f, 0.f, 0.f, 1.f));
}

void UZeroCrossHairWidget::UnHitCrosshair()
{
    CrosshairLeft->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
    CrosshairRight->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
    CrosshairTop->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
    CrosshairBottom->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
}
