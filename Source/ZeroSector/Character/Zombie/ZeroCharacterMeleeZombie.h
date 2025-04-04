// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "ZeroZombieType.h"
#include "ZeroCharacterMeleeZombie.generated.h"

class UZeroZombieBehaviorComponent;

DECLARE_DELEGATE(FZombieAttackOne)
DECLARE_DELEGATE(FZombieAttackTwo)
DECLARE_DELEGATE(FZombieDead)

USTRUCT()
struct FZombieAttackOneWrapper
{
	GENERATED_BODY()

public:
	FZombieAttackOneWrapper() {}
	FZombieAttackOneWrapper(const FZombieAttackOne& InZombieAttackOne) : ZombieAttackOne(InZombieAttackOne) {}

	FZombieAttackOne ZombieAttackOne;
};

USTRUCT()
struct FZombieAttackTwoWrapper
{
	GENERATED_BODY()

public:
	FZombieAttackTwoWrapper() {}
	FZombieAttackTwoWrapper(const FZombieAttackTwo& InZombieAttackTwo) : ZombieAttackTwo(InZombieAttackTwo) {}

	FZombieAttackTwo ZombieAttackTwo;
};

USTRUCT()
struct FZombieDeadWrapper
{
	GENERATED_BODY()

public:
	FZombieDeadWrapper() {}
	FZombieDeadWrapper(const FZombieDead& InZombieDead) : ZombieDead(InZombieDead) {}

	FZombieDead ZombieDead;
};

UCLASS()
class ZEROSECTOR_API AZeroCharacterMeleeZombie : public AZeroCharacterBaseZombie
{
	GENERATED_BODY()
	
public:
	AZeroCharacterMeleeZombie();

	/* IZeroCharacterAIInterface Implement */
	virtual void AttackOneByAI() override;
	virtual void AttackTwoByAI() override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;
	
/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

/* Behavior */
private:
	void BeginAttackOne();
	void BeginAttackOneByCommon();
	void BeginAttackOneByTanker();
	void BeginAttackOneBySpeed();
	void EndAttackOne();

	void BeginAttackTwo();
	void BeginAttackTwoByCommon();
	void BeginAttackTwoByTanker();
	void BeginAttackTwoBySpeed();
	void EndAttackTwo();


	void BeginDead();
	void BeginDeadByCommon();
	void BeginDeadByTanker();
	void BeginDeadBySpeed();
	
	UPROPERTY()
	TMap<EZombieType, FZombieAttackOneWrapper> ZombieAttackOneMaps;

	UPROPERTY()
	TMap<EZombieType, FZombieAttackTwoWrapper> ZombieAttackTwoMaps;

	UPROPERTY()
	TMap<EZombieType, FZombieDeadWrapper> ZombieDeadMaps;




private:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	TObjectPtr<UZeroZombieBehaviorComponent> BehaviorComp;

	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EZombieType CurrentType;
};
