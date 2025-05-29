// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "ZeroZombieType.h"
#include "Data/Animation/ZeroZombieAnimDataTable.h"
#include "ZeroCharacterMeleeZombie.generated.h"

class UAnimMontage;
class UAnimInstance;

UCLASS()
class ZEROSECTOR_API AZeroCharacterMeleeZombie : public AZeroCharacterBaseZombie
{
	GENERATED_BODY()
	
public:
	AZeroCharacterMeleeZombie();

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
	
/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


/* Spawn */
public:
	virtual void Landed(const FHitResult& Hit) override;
	void SpawnInit(const FVector& Direction);


protected:
	virtual void BeginPlay() override;

/* Behavior */
private:
	void BeginAttackOne();
	void EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginAttackTwo();
	void EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginDead();
	
	UPROPERTY(EditDefaultsOnly, Category = "Type")
	EZombieType CurrentType;

// 애니메이션 데이터 
private:
	UAnimMontage* GetAttackOneMontage() const;
	UAnimMontage* GetAttackTwoMontage() const;

	UPROPERTY()
	FZeroZombieAnimDataTable ZeroZombieAnimDataTable;

	UPROPERTY()
	TObjectPtr<UDataTable> DataTableBuffer;

	UPROPERTY()
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> TempMontage;
};
