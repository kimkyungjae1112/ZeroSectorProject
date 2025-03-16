// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterBase.h"

AZeroCharacterBase::AZeroCharacterBase()
{

}

FGenericTeamId AZeroCharacterBase::GetGenericTeamId() const
{
	return FGenericTeamId();
}

void AZeroCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


