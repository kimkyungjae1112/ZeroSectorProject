// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroAfternoonPlayerStart.h"
#include "Components/CapsuleComponent.h"

AZeroAfternoonPlayerStart::AZeroAfternoonPlayerStart()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComp;
}


