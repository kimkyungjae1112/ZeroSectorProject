// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIZombieTest.h"
#include "Components/CapsuleComponent.h"

AZeroCharacterAIZombieTest::AZeroCharacterAIZombieTest()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));

	TeamId = FGenericTeamId(1);

	bIsRangedZombie = false;


}

FGenericTeamId AZeroCharacterAIZombieTest::GetGenericTeamId() const
{
	return TeamId;
}

void AZeroCharacterAIZombieTest::ApplyDamage(float Damage)
{
	UE_LOG(LogTemp, Warning, TEXT("Zombie took damage: %f"), Damage);

	Super::ApplyDamage(Damage);
}

float AZeroCharacterAIZombieTest::GetAIAttackRange()
{
	return 200.0f;
}

float AZeroCharacterAIZombieTest::GetAITurnSpeed()
{
	return 2.0f;
}

void AZeroCharacterAIZombieTest::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AZeroCharacterAIZombieTest::AttackByAI()
{
	// 임시로 1초로 설정해둠

	GetWorld()->GetTimerManager().SetTimer(
		AttackFinishTimerHandle,
		this,
		&AZeroCharacterAIZombieTest::OnAttackFinishedInternal,
		1.0f,
		false
	);
}

void AZeroCharacterAIZombieTest::OnAttackFinishedInternal()
{

	if (OnAttackFinished.IsBound())
	{
		OnAttackFinished.Execute();
	}
}