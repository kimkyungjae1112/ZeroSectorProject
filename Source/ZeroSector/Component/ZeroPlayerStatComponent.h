// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ZeroStatComponent.h"
#include "ZeroPlayerStatComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangedActivePoint, float /* ActivePoint */)

UCLASS()
class ZEROSECTOR_API UZeroPlayerStatComponent : public UZeroStatComponent
{
	GENERATED_BODY()
	
public:
	UZeroPlayerStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxActivePoint() const { return MaxActivePoint; }

	FOnChangedActivePoint OnChangedActivePoint;

	void InitActivePoint();
	void InitHealth();
	void UseActivePoint(float UsedActivePoint);

private:
	UPROPERTY(VisibleAnywhere, Category = "ActivePoint")
	float MaxActivePoint;

	UPROPERTY(VisibleAnywhere, Category = "ActivePoint")
	float CurrentActivePoint;
};
