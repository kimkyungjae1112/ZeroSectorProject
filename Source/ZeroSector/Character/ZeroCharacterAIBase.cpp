// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIBase.h"

AZeroCharacterAIBase::AZeroCharacterAIBase()
{
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}
