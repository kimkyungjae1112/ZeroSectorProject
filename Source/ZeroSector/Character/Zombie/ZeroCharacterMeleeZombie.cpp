// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterMeleeZombie.h"
#include "Component/ZeroZombieBehaviorComponent.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ZeroGameModeBase.h"
#include "AI/Controller/ZeroAIControllerMeleeZombie.h"
#include "ZeroSector.h"

AZeroCharacterMeleeZombie::AZeroCharacterMeleeZombie()
{
	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Melee");
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));

	ZombieAttackOneMaps.Add(EZombieType::EZ_Common, FZombieAttackOneWrapper(FZombieAttackOne::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginAttackOneByCommon)));
	ZombieAttackOneMaps.Add(EZombieType::EZ_Tanker, FZombieAttackOneWrapper(FZombieAttackOne::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginAttackOneByTanker)));
	ZombieAttackOneMaps.Add(EZombieType::EZ_Speed, FZombieAttackOneWrapper(FZombieAttackOne::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginAttackOneBySpeed)));

	ZombieAttackTwoMaps.Add(EZombieType::EZ_Common, FZombieAttackTwoWrapper(FZombieAttackTwo::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginAttackTwoByCommon)));
	ZombieAttackTwoMaps.Add(EZombieType::EZ_Tanker, FZombieAttackTwoWrapper(FZombieAttackTwo::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginAttackTwoByTanker)));
	ZombieAttackTwoMaps.Add(EZombieType::EZ_Speed, FZombieAttackTwoWrapper(FZombieAttackTwo::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginAttackTwoBySpeed)));

	ZombieDeadMaps.Add(EZombieType::EZ_Common, FZombieDeadWrapper(FZombieDead::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginDeadByCommon)));
	ZombieDeadMaps.Add(EZombieType::EZ_Tanker, FZombieDeadWrapper(FZombieDead::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginDeadByTanker)));
	ZombieDeadMaps.Add(EZombieType::EZ_Speed, FZombieDeadWrapper(FZombieDead::CreateUObject(this, &AZeroCharacterMeleeZombie::BeginDeadBySpeed)));

	BehaviorComp = CreateDefaultSubobject<UZeroZombieBehaviorComponent>(TEXT("Behavior Component"));
	BehaviorComp->NotifyFinishedAttackOne.BindUObject(this, &AZeroCharacterMeleeZombie::EndAttackOne);
	BehaviorComp->NotifyFinishedAttackTwo.BindUObject(this, &AZeroCharacterMeleeZombie::EndAttackTwo);

	StatComp->OnHpZero.AddUObject(this, &AZeroCharacterMeleeZombie::BeginDead);
}

void AZeroCharacterMeleeZombie::AttackOneByAI()
{
	Super::AttackOneByAI();

	BeginAttackOne();
}

void AZeroCharacterMeleeZombie::AttackTwoByAI()
{
	Super::AttackTwoByAI();

	BeginAttackOne();
}

AController* AZeroCharacterMeleeZombie::GetAIController()
{
	return GetController();
}

FGenericTeamId AZeroCharacterMeleeZombie::GetGenericTeamId() const
{
	return TeamId;
}

float AZeroCharacterMeleeZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamage(Damage);

	return 0.0f;
}

AZeroAIControllerMeleeZombie* AZeroCharacterMeleeZombie::GetMyController()
{
	return CastChecked<AZeroAIControllerMeleeZombie>(GetController());
}

void AZeroCharacterMeleeZombie::BeginAttackOne()
{
	ZombieAttackOneMaps[CurrentType].ZombieAttackOne.ExecuteIfBound();
}

void AZeroCharacterMeleeZombie::BeginAttackOneByCommon()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackOne By Common"));
	BehaviorComp->BeginAttackOne();
}

void AZeroCharacterMeleeZombie::BeginAttackOneByTanker()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackOne By Tanker"));
	BehaviorComp->BeginAttackOne();
}

void AZeroCharacterMeleeZombie::BeginAttackOneBySpeed()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackOne By Speed"));
	BehaviorComp->BeginAttackOne();
}

void AZeroCharacterMeleeZombie::EndAttackOne()
{
	OnAttackOneFinished.ExecuteIfBound();
}

void AZeroCharacterMeleeZombie::BeginAttackTwo()
{
	ZombieAttackTwoMaps[CurrentType].ZombieAttackTwo.ExecuteIfBound();
}

void AZeroCharacterMeleeZombie::BeginAttackTwoByCommon()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackTwo By Common"));
	BehaviorComp->BeginAttackTwo();
}

void AZeroCharacterMeleeZombie::BeginAttackTwoByTanker()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackTwo By Tanker"));
	BehaviorComp->BeginAttackTwo();
}

void AZeroCharacterMeleeZombie::BeginAttackTwoBySpeed()
{
	ZE_LOG(LogZeroSector, Display, TEXT("AttackTwo By Speed"));
	BehaviorComp->BeginAttackTwo();
}

void AZeroCharacterMeleeZombie::EndAttackTwo()
{
	OnAttackTwoFinished.ExecuteIfBound();
}

void AZeroCharacterMeleeZombie::BeginDead()
{
	DetachFromControllerPendingDestroy();

	ZombieDeadMaps[CurrentType].ZombieDead.ExecuteIfBound();
	ZE_LOG(LogZeroSector, Display, TEXT("Zombie Dead"));

	SetActorEnableCollision(false);
	
	FTimerHandle DestoryTimer;
	GetWorld()->GetTimerManager().SetTimer(DestoryTimer, [&]()
		{
			Destroy();
		}, 1.5f, false);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PawnKilled(this);
	}
}

void AZeroCharacterMeleeZombie::BeginDeadByCommon()
{
	BehaviorComp->BeginDead();
}

void AZeroCharacterMeleeZombie::BeginDeadByTanker()
{
	BehaviorComp->BeginDead();
}

void AZeroCharacterMeleeZombie::BeginDeadBySpeed()
{
	BehaviorComp->BeginDead();
}
