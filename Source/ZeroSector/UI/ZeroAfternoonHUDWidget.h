// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroAfternoonHUDWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroAfternoonHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroAfternoonHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void SetMaxActivePoint(float InMaxActivePoint);
	void UpdateAPBar(float InActivePoint);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProgressBar> ActivePointBar;

	float MaxActivePoint;

public:
	UFUNCTION(BlueprintCallable)
	void ShowInterviewText(const FString& ResearcherName);

	UFUNCTION(BlueprintCallable)
	void HideInterviewText();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InterviewText;
};
