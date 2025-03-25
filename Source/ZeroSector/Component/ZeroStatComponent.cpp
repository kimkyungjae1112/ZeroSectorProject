// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroStatComponent.h"

UZeroStatComponent::UZeroStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UZeroStatComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UZeroStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

