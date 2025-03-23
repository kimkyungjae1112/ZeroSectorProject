// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ZeroPlayerCameraData.generated.h"

UCLASS()
class ZEROSECTOR_API UZeroPlayerCameraData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	FVector CommonCameraVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Common")
	FRotator CommonCameraRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "In Dialogue")
	FVector DialogueCameraVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "In Dialogue")
	FRotator DialogueCameraRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	FVector UnAimingCameraVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	FRotator UnAimingCameraRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	FVector AimingCameraVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	FRotator AimingCameraRotator;
};
