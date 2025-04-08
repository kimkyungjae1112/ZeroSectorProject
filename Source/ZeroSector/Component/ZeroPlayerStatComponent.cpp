// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroPlayerStatComponent.h"
#include "ZeroSector.h"

UZeroPlayerStatComponent::UZeroPlayerStatComponent()
{
	MaxActivePoint = 100.f;
	CurrentActivePoint = MaxActivePoint;
}

void UZeroPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UZeroPlayerStatComponent::InitActivePoint()
{
	CurrentActivePoint = MaxActivePoint;
}

void UZeroPlayerStatComponent::InitHealth()
{
	SetHp(BaseStat.MaxHp);
}

void UZeroPlayerStatComponent::UseActivePoint(float UsedActivePoint)
{
	CurrentActivePoint += UsedActivePoint;
	ZE_LOG(LogZeroSector, Display, TEXT("사용되고 남은 AP : %f"), CurrentActivePoint);
	if (CurrentActivePoint < KINDA_SMALL_NUMBER)
	{
		CurrentActivePoint = 0.f;
	}
	OnChangedActivePoint.ExecuteIfBound(CurrentActivePoint);
}
