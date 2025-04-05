// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ZeroInputConfig.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ZEROSECTOR_API UZeroInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Day;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mapping Context")
	TObjectPtr<UInputMappingContext> IMC_Night;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Interact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Fire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Aiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_ChangeWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_NightToAfternoon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_ToggleNote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Run;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_PauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_ExcludeResearcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Action")
	TObjectPtr<UInputAction> IA_SelectResearcher;
};
