// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterRangedZombie.h"
#include "Component/ZeroStatComponent.h"

AZeroCharacterRangedZombie::AZeroCharacterRangedZombie()
{

	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Ranged");
}

FGenericTeamId AZeroCharacterRangedZombie::GetGenericTeamId() const
{
	return TeamId;
}

float AZeroCharacterRangedZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamage(Damage);

	return 0.0f;
}
