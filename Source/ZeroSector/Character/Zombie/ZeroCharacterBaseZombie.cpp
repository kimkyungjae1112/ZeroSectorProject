// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterBaseZombie.h"               
#include "Component/ZeroStatComponent.h"

AZeroCharacterBaseZombie::AZeroCharacterBaseZombie()
{
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    StatComp = CreateDefaultSubobject<UZeroStatComponent>(TEXT("Stat Component"));
}

void AZeroCharacterBaseZombie::BeginPlay()
{
    Super::BeginPlay();

}

float AZeroCharacterBaseZombie::GetAIAttackRange()
{
    return 0.0f;
}

float AZeroCharacterBaseZombie::GetAITurnSpeed()
{
    return 0.0f;
}

void AZeroCharacterBaseZombie::SetAIAttackOneDelegate(const FOnAttackOneFinished& InOnAttackFinished)
{
    OnAttackOneFinished = InOnAttackFinished;
}

void AZeroCharacterBaseZombie::SetAIAttackTwoDelegate(const FOnAttackOneFinished& InOnAttackTwoFinished)
{
    OnAttackTwoFinished = InOnAttackTwoFinished;
}

void AZeroCharacterBaseZombie::AttackOneByAI()
{
}

void AZeroCharacterBaseZombie::AttackTwoByAI()
{
}

float AZeroCharacterBaseZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

    return 0.0f;
}
