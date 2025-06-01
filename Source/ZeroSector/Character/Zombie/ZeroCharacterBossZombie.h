// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Zombie/ZeroCharacterBaseZombie.h"
#include "ZeroCharacterBossZombie.generated.h"

class UDecalComponent;

UCLASS()
class ZEROSECTOR_API AZeroCharacterBossZombie : public AZeroCharacterBaseZombie
{
	GENERATED_BODY()

public:
	AZeroCharacterBossZombie();

	/* IZeroCharacterAIInterface Implement */
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual float GetRunSpeed() override;
	virtual float GetWalkSpeed() override;

	virtual void SetAISpawnAttackDelegate(const FOnSpawnAttackFinished& InOnSpawnAttackFinished) override;

	virtual void AttackOneByAI() override;
	virtual void AttackTwoByAI() override;

	// Not Super
	virtual void SpawnAttackByAI() override;

	virtual AController* GetAIController() override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

/* Damaged */
public:
	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

/* Behavior */
private:
	void BeginAttackOne();
	void EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginRushAttack();
	void EndRushAttack(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginSpawnAttack();
	void EndSpawnAttack(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginDead();

// 애니메이션 데이터 
private:
	UPROPERTY(VisibleAnywhere, Category = "Anim")
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY(EditAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> AttackOneMontage;

	UPROPERTY(EditAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> RushAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> SpawnAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Anim")
	TObjectPtr<UAnimMontage> DeadMontage;

// Decal
private:
	void ShowDangerDecal(FVector AttackLocation, float Radius, float Duration);

	UPROPERTY()
	TObjectPtr<UDecalComponent> DangerDecal;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	TObjectPtr<UMaterialInterface> DangerMaterial;

private:
	FOnSpawnAttackFinished OnSpawnAttackFinished;
};
