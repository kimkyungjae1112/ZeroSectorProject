// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterAIRangedZombie.h"
#include "Component/ZeroStatComponent.h"

AZeroCharacterAIRangedZombie::AZeroCharacterAIRangedZombie()
{

	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Ranged");
}

FGenericTeamId AZeroCharacterAIRangedZombie::GetGenericTeamId() const
{
	return TeamId;
}

float AZeroCharacterAIRangedZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamage(Damage);

	return 0.0f;
}
