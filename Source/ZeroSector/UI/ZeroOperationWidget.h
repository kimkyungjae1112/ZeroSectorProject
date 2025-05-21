// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroOperationWidget.generated.h"

class UButton;
class UZeroGameInstance;

DECLARE_DELEGATE(FOnClickNextButton)
DECLARE_DELEGATE(FOnCancelButton)

UCLASS()
class ZEROSECTOR_API UZeroOperationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UZeroOperationWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetDelegateClickNextButton(const FOnClickNextButton& InOnClickNextButton) { OnClickNextButton = InOnClickNextButton; }
	FORCEINLINE EWeaponType GetWeaponType() const { return CurrentWeaponType; }

	FOnCancelButton OnCancelButton;

private:
	UFUNCTION()
	void ClickRifleButton();

	UFUNCTION()
	void ClickShotgunButton();

	UFUNCTION()
	void ClickNextButton();

	UFUNCTION()
	void ClickCancelButton();

private:
	FOnClickNextButton OnClickNextButton;

	UPROPERTY(VisibleAnywhere)
	EWeaponType CurrentWeaponType;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> RifleButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> ShotgunButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> NextButton;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UButton> CancelButton;

	bool bIsSeleteced{ false };

	UPROPERTY()
	TObjectPtr<UZeroGameInstance> GI;

};
