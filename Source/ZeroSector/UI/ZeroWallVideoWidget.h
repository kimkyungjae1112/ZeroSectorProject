// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroWallVideoWidget.generated.h"

class UMediaPlayer;
class UMediaSource;

UCLASS()
class ZEROSECTOR_API UZeroWallVideoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroWallVideoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void VideoFinished();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMediaPlayer> PrologMediaPlayer;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMediaSource> PrologMediaSource;
};
