// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterAIBase.h"
#include "ZeroZombieType.h"
#include "ZeroCharacterAIMeleeZombie.generated.h"

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
class ZEROSECTOR_API AZeroCharacterAIMeleeZombie : public AZeroCharacterAIBase
{
	GENERATED_BODY()
	
public:
	AZeroCharacterAIMeleeZombie();

	/* IZeroCharacterAIInterface Implement */
	virtual void AttackOneByAI() override;
	virtual void AttackTwoByAI() override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

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
