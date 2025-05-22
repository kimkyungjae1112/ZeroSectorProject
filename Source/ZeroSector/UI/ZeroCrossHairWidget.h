// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroCrossHairWidget.generated.h"

class UImage;

UCLASS()
class ZEROSECTOR_API UZeroCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroCrossHairWidget(const FObjectInitializer& ObjectInitializer);

    FORCEINLINE float GetCurrentSpread() const { return CrosshairSpread; }

protected:
	virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
    // 총 발사 시 스프레드를 증가시키는 함수
    void IncreaseSpread(float Amount);

    void HitCrosshair();
    void UnHitCrosshair();

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CrosshairLeft;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CrosshairRight;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CrosshairTop;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> CrosshairBottom;


    // 크로스 헤어 스프레드 값 (총 발사 시 증가)
    float CrosshairSpread;

    // 스프레드가 원래대로 돌아가는 속도
    float CrosshairRecoverySpeed;

    float MaxCrosshairSpread;
    
};
