// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroDialogueCamera.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroDialogueCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroDialogueCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* DialogueCamera;

};
