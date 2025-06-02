// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroEndingVideoWidget.generated.h"

class UMediaPlayer;
class UMediaSource;

UCLASS()
class ZEROSECTOR_API UZeroEndingVideoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroEndingVideoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void VideoFinished();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMediaPlayer> EndingMediaPlayer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMediaSource> EndingMediaSource;
};
