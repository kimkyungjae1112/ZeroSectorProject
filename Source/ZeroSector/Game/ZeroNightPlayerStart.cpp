// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroNightPlayerStart.h"
#include "Components/CapsuleComponent.h"

AZeroNightPlayerStart::AZeroNightPlayerStart()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComp;
}



