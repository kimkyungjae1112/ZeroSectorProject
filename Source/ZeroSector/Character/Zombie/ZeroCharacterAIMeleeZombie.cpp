// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterAIMeleeZombie.h"

AZeroCharacterAIMeleeZombie::AZeroCharacterAIMeleeZombie()
{
	
	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Melee");
}

FGenericTeamId AZeroCharacterAIMeleeZombie::GetGenericTeamId() const
{
	return TeamId;
}
