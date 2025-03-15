// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIZombieTest.h"

AZeroCharacterAIZombieTest::AZeroCharacterAIZombieTest()
{
	TeamId = FGenericTeamId(1);
}

FGenericTeamId AZeroCharacterAIZombieTest::GetGenericTeamId() const
{
	return TeamId;
}
