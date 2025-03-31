// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIBase.h"               
#include "Component/ZeroStatComponent.h"

AZeroCharacterAIBase::AZeroCharacterAIBase()
{
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    StatComp = CreateDefaultSubobject<UZeroStatComponent>(TEXT("Stat Component"));
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

void AZeroCharacterAIBase::SetAIAttackOneDelegate(const FOnAttackOneFinished& InOnAttackFinished)
{
    OnAttackOneFinished = InOnAttackFinished;
}

void AZeroCharacterAIBase::SetAIAttackTwoDelegate(const FOnAttackOneFinished& InOnAttackTwoFinished)
{
    OnAttackTwoFinished = InOnAttackTwoFinished;
}

void AZeroCharacterAIBase::AttackOneByAI()
{
}

void AZeroCharacterAIBase::AttackTwoByAI()
{
}

float AZeroCharacterAIBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    return 0.0f;
}
