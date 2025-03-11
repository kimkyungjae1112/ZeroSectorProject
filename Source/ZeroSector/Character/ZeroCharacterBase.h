// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZeroCharacterBase.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AZeroCharacterBase();

protected:
	virtual void BeginPlay() override;


};
