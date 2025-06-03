// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterBaseZombie.h"               
#include "Component/ZeroStatComponent.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "GameFramework/CharacterMovementComponent.h"

AZeroCharacterBaseZombie::AZeroCharacterBaseZombie()
{
    GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
    GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    GetCharacterMovement()->MaxWalkSpeed = 250.f;

    StatComp = CreateDefaultSubobject<UZeroStatComponent>(TEXT("Stat Component"));
}

void AZeroCharacterBaseZombie::BeginPlay()
{
    Super::BeginPlay();

    UZeroAnimInstanceZombie* ZombieAnim = Cast<UZeroAnimInstanceZombie>(GetMesh()->GetAnimInstance());
    if (ZombieAnim)
    {
        AnimPoseType = static_cast<int32>(FMath::RandRange(0, 3));
        AnimIndex = static_cast<int32>(FMath::RandRange(0, 3));

        ZombieAnim->AnimPoseType = AnimPoseType;
        ZombieAnim->AnimIndex = AnimIndex;
    }

}

float AZeroCharacterBaseZombie::GetAIAttackRange()
{
    return 0.0f;
}

float AZeroCharacterBaseZombie::GetAITurnSpeed()
{
    return 0.0f;
}

float AZeroCharacterBaseZombie::GetRunSpeed()
{
    return StatComp->GetTotalStat().RunSpeed;
}

float AZeroCharacterBaseZombie::GetWalkSpeed()
{
    return StatComp->GetTotalStat().WalkSpeed;
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
