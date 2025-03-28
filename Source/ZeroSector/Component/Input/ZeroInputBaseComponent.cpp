// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Input/ZeroInputBaseComponent.h"
#include "GameFramework/Character.h"

void UZeroInputBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = CastChecked<ACharacter>(GetOwner());
}
