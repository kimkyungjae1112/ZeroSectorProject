// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "ZeroZombieType.h"
#include "Data/Animation/ZeroZombieAnimDataTable.h"
#include "ZeroCharacterMeleeZombie.generated.h"

class UAnimMontage;
class UAnimInstance;

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

	virtual AController* GetAIController() override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;
	
/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

/* Util */
private:

/* Behavior */
private:
	void BeginAttackOne();
	void EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginAttackTwo();
	void EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginDead();

	
	UPROPERTY()
	TMap<EZombieType, FZombieAttackOneWrapper> ZombieAttackOneMaps;

	UPROPERTY()
	TMap<EZombieType, FZombieAttackTwoWrapper> ZombieAttackTwoMaps;

	UPROPERTY()
	TMap<EZombieType, FZombieDeadWrapper> ZombieDeadMaps;

	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EZombieType CurrentType;

// 애니메이션 데이터 
private:
	UAnimMontage* GetAttackOneMontage() const;
	UAnimMontage* GetAttackTwoMontage() const;
	UAnimMontage* GetDeadMontage() const;

	UPROPERTY()
	FZeroZombieAnimDataTable ZeroZombieAnimDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> DataTableBuffer;

	UPROPERTY()
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> TempMontage;
};
