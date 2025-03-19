// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIZombieTest.h"
#include "Components/CapsuleComponent.h"

AZeroCharacterAIZombieTest::AZeroCharacterAIZombieTest()
{
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));

	TeamId = FGenericTeamId(1);
}

FGenericTeamId AZeroCharacterAIZombieTest::GetGenericTeamId() const
{
	return TeamId;
}
