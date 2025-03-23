// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIRangedZombieTest.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"     
#include "TimerManager.h"

AZeroCharacterAIRangedZombieTest::AZeroCharacterAIRangedZombieTest()
{
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	bIsRangedZombie = true;
}

FGenericTeamId AZeroCharacterAIRangedZombieTest::GetGenericTeamId() const
{
	return FGenericTeamId(1); 
}

void AZeroCharacterAIRangedZombieTest::ApplyDamage(float Damage)
{
	Super::ApplyDamage(Damage);

}

float AZeroCharacterAIRangedZombieTest::GetAIAttackRange()
{
	return 800.f; 
}

float AZeroCharacterAIRangedZombieTest::GetAITurnSpeed()
{
	return 180.f; 
}

void AZeroCharacterAIRangedZombieTest::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}


void AZeroCharacterAIRangedZombieTest::AttackByAI()
{
	FireProjectile();

	GetWorld()->GetTimerManager().SetTimer(
		AttackFinishTimerHandle,
		this,
		&AZeroCharacterAIRangedZombieTest::OnAttackFinishedInternal,
		1.0f, 
		false
	);

}


void AZeroCharacterAIRangedZombieTest::OnAttackFinishedInternal()
{

	if (OnAttackFinished.IsBound())
	{
		OnAttackFinished.Execute();
	}
}

void AZeroCharacterAIRangedZombieTest::FireProjectile()
{
}