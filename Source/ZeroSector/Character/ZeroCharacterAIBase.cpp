// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIBase.h"               
#include "AIController.h"                         

AZeroCharacterAIBase::AZeroCharacterAIBase()
{
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}



void AZeroCharacterAIBase::BeginPlay()
{
    Super::BeginPlay();

}

float AZeroCharacterAIBase::GetAIAttackRange()
{
    return 0.0f;
}

float AZeroCharacterAIBase::GetAITurnSpeed()
{
    return 0.0f;
}

void AZeroCharacterAIBase::SetAIAttackDelegate(const FOnAttackFinished& InOnAttackFinished)
{
    OnAttackFinished = InOnAttackFinished;
}

void AZeroCharacterAIBase::AttackByAI()
{
}
