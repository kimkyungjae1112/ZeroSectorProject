// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroPlayerStatComponent.h"
#include "ZeroSector.h"

UZeroPlayerStatComponent::UZeroPlayerStatComponent()
{
}

void UZeroPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	ZE_LOG(LogZeroSector, Display, TEXT("%f"), CurrentHp);
}
