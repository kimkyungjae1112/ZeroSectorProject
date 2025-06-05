// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "Data/Animation/ZeroZombieAnimDataTable.h"
#include "Interface/ZeroRangedZombieAttackInterface.h"
#include "ZeroCharacterRangedZombie.generated.h"


class UAnimMontage;
class UAnimInstance;
class AZeroAIControllerRangedZombie;
class AZeroRangedZombieProjectile;

UCLASS()
class ZEROSECTOR_API AZeroCharacterRangedZombie 
	: public AZeroCharacterBaseZombie
	, public IZeroRangedZombieAttackInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterRangedZombie();

	/* IZeroCharacterAIInterface Implement */
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual float GetRunSpeed() override;
	virtual float GetWalkSpeed() override;

	virtual void AttackOneByAI() override;
	virtual void AttackTwoByAI() override;

	virtual AController* GetAIController() override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

	/* IZeroRangedZombieAttackInterface Implement */
	virtual void SpawnProjectile() override;

/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

/* Util */
private:
	AZeroAIControllerRangedZombie* GetMyController() const;

/* Behavior */
private:
	void BeginAttackOne();
	void EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginAttackTwo();
	void EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginDead();

	void BeginHitReaction();
	void EndHitReaction(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginStagger();

/* 투사체 */
private:
	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AZeroRangedZombieProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	TObjectPtr<AZeroRangedZombieProjectile> Projectile;


// 애니메이션 데이터 
private:
	UAnimMontage* GetHitReactionMontage() const;
	UAnimMontage* GetStaggerMontage() const;

	UPROPERTY()
	FZeroZombieAnimDataTable ZeroZombieAnimDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> DataTableBuffer;

	UPROPERTY()
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> TempMontage;

// 사운드
	private:
	FTimerHandle ZombieGrowlTimer;

	void PlayZombieMove();
	void ScheduleNextMove();
};
