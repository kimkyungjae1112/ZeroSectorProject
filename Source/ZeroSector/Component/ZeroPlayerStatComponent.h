// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ZeroStatComponent.h"
#include "ZeroPlayerStatComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangedActivePoint, float /* ActivePoint */)
DECLARE_MULTICAST_DELEGATE(FOnZeroActivePoint)

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
	FORCEINLINE bool GetCanInteract() const { return bCanInteract; }

	FOnChangedActivePoint OnChangedActivePoint;
	static FOnZeroActivePoint OnZeroActivePoint;

	void InitActivePoint();
	void InitHealth();
	void UseActivePoint(float UsedActivePoint);

private:
	UPROPERTY(VisibleAnywhere, Category = "ActivePoint")
	float MaxActivePoint;

	UPROPERTY(VisibleAnywhere, Category = "ActivePoint")
	float CurrentActivePoint;

	bool bCanInteract{ true };
};
