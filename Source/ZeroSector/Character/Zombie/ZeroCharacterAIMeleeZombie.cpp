// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterAIMeleeZombie.h"
#include "Component/ZeroZombieBehaviorComponent.h"
#include "Component/ZeroStatComponent.h"
#include "ZeroSector.h"

AZeroCharacterAIMeleeZombie::AZeroCharacterAIMeleeZombie()
{
	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Melee");

	ZombieAttackOneMaps.Add(EZombieType::EZ_Common, FZombieAttackOneWrapper(FZombieAttackOne::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginAttackOneByCommon)));
	ZombieAttackOneMaps.Add(EZombieType::EZ_Tanker, FZombieAttackOneWrapper(FZombieAttackOne::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginAttackOneByTanker)));
	ZombieAttackOneMaps.Add(EZombieType::EZ_Speed, FZombieAttackOneWrapper(FZombieAttackOne::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginAttackOneBySpeed)));

	ZombieAttackTwoMaps.Add(EZombieType::EZ_Common, FZombieAttackTwoWrapper(FZombieAttackTwo::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginAttackTwoByCommon)));
	ZombieAttackTwoMaps.Add(EZombieType::EZ_Tanker, FZombieAttackTwoWrapper(FZombieAttackTwo::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginAttackTwoByTanker)));
	ZombieAttackTwoMaps.Add(EZombieType::EZ_Speed, FZombieAttackTwoWrapper(FZombieAttackTwo::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginAttackTwoBySpeed)));

	ZombieDeadMaps.Add(EZombieType::EZ_Common, FZombieDeadWrapper(FZombieDead::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginDeadByCommon)));
	ZombieDeadMaps.Add(EZombieType::EZ_Tanker, FZombieDeadWrapper(FZombieDead::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginDeadByTanker)));
	ZombieDeadMaps.Add(EZombieType::EZ_Speed, FZombieDeadWrapper(FZombieDead::CreateUObject(this, &AZeroCharacterAIMeleeZombie::BeginDeadBySpeed)));

	BehaviorComp = CreateDefaultSubobject<UZeroZombieBehaviorComponent>(TEXT("Behavior Component"));
	BehaviorComp->NotifyFinishedAttackOne.BindUObject(this, &AZeroCharacterAIMeleeZombie::EndAttackOne);
	BehaviorComp->NotifyFinishedAttackTwo.BindUObject(this, &AZeroCharacterAIMeleeZombie::EndAttackTwo);

	StatComp->OnHpZero.AddUObject(this, &AZeroCharacterAIMeleeZombie::BeginDead);
}

void AZeroCharacterAIMeleeZombie::AttackOneByAI()
{
	Super::AttackOneByAI();

	BeginAttackOne();
}

void AZeroCharacterAIMeleeZombie::AttackTwoByAI()
{
	Super::AttackTwoByAI();

	BeginAttackOne();
}

FGenericTeamId AZeroCharacterAIMeleeZombie::GetGenericTeamId() const
{
	return TeamId;
}

void AZeroCharacterAIMeleeZombie::BeginAttackOne()
{
	ZombieAttackOneMaps[CurrentType].ZombieAttackOne.ExecuteIfBound();
}

void AZeroCharacterAIMeleeZombie::BeginAttackOneByCommon()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackOne By Common"));
	BehaviorComp->BeginAttackOne();
}

void AZeroCharacterAIMeleeZombie::BeginAttackOneByTanker()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackOne By Tanker"));
	BehaviorComp->BeginAttackOne();
}

void AZeroCharacterAIMeleeZombie::BeginAttackOneBySpeed()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackOne By Speed"));
	BehaviorComp->BeginAttackOne();
}

void AZeroCharacterAIMeleeZombie::EndAttackOne()
{
	OnAttackOneFinished.ExecuteIfBound();
}

void AZeroCharacterAIMeleeZombie::BeginAttackTwo()
{
	ZombieAttackTwoMaps[CurrentType].ZombieAttackTwo.ExecuteIfBound();
}

void AZeroCharacterAIMeleeZombie::BeginAttackTwoByCommon()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackTwo By Common"));
	BehaviorComp->BeginAttackTwo();
}

void AZeroCharacterAIMeleeZombie::BeginAttackTwoByTanker()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackTwo By Tanker"));
	BehaviorComp->BeginAttackTwo();
}

void AZeroCharacterAIMeleeZombie::BeginAttackTwoBySpeed()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackTwo By Speed"));
	BehaviorComp->BeginAttackTwo();
}

void AZeroCharacterAIMeleeZombie::EndAttackTwo()
{
	OnAttackTwoFinished.ExecuteIfBound();
}

void AZeroCharacterAIMeleeZombie::BeginDead()
{
	ZombieDeadMaps[CurrentType].ZombieDead.ExecuteIfBound();
}

void AZeroCharacterAIMeleeZombie::BeginDeadByCommon()
{
	BehaviorComp->BeginDead();
}

void AZeroCharacterAIMeleeZombie::BeginDeadByTanker()
{
	BehaviorComp->BeginDead();
}

void AZeroCharacterAIMeleeZombie::BeginDeadBySpeed()
{
	BehaviorComp->BeginDead();
}
