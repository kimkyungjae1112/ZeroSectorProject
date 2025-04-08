// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ZeroGimmick.h"
#include "ZeroWaveTrigger.generated.h"

class UBoxComponent;

UCLASS()
class ZEROSECTOR_API AZeroWaveTrigger : public AZeroGimmick
{
	GENERATED_BODY()
	
public:	
	AZeroWaveTrigger();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
