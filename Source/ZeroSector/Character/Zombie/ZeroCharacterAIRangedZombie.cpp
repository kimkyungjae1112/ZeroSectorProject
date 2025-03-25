// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterAIRangedZombie.h"

AZeroCharacterAIRangedZombie::AZeroCharacterAIRangedZombie()
{

	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Ranged");
}

FGenericTeamId AZeroCharacterAIRangedZombie::GetGenericTeamId() const
{
	return TeamId;
}
