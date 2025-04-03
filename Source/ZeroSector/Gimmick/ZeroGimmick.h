// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroGimmick.generated.h"

class UBoxComponent;

UCLASS(abstract)
class ZEROSECTOR_API AZeroGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroGimmick();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<UBoxComponent> BoxComp;

};
