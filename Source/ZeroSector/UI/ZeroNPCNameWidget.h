// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZeroNPCNameWidget.generated.h"

class UTextBlock;

UCLASS()
class ZEROSECTOR_API UZeroNPCNameWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UZeroNPCNameWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetOwningActor(AActor* Owner) { OwningActor = Owner; }

protected:
	virtual void NativeConstruct() override;
	void SetNameText(const FName& InName);

private:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	TObjectPtr<AActor> OwningActor;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTextBlock> NameText;
};
