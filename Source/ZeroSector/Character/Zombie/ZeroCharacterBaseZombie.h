// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "Interface/ZeroCharacterAIInterface.h"
#include "ZeroCharacterBaseZombie.generated.h"

class UZeroStatComponent;
class UNiagarSystem;

UCLASS(abstract)
class ZEROSECTOR_API AZeroCharacterBaseZombie
	: public AZeroCharacterBase
	, public IZeroCharacterAIInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterBaseZombie();

	/* IZeroCharacterAIInterface Implement */
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
	virtual float GetRunSpeed() override;
	virtual float GetWalkSpeed() override;

	virtual void SetAIAttackOneDelegate(const FOnAttackOneFinished& InOnAttackOneFinished) override;
	virtual void SetAIAttackTwoDelegate(const FOnAttackOneFinished& InOnAttackTwoFinished) override;
	virtual void AttackOneByAI() override;
	virtual void AttackTwoByAI() override;

	virtual AController* GetAIController() override { return nullptr; }

	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Stat")
	TObjectPtr<UZeroStatComponent> StatComp;

// 공격 끝 감지
protected:
	FOnAttackOneFinished OnAttackOneFinished;
	FOnAttackTwoFinished OnAttackTwoFinished;

// 애니메이션 타입 결정
protected:
	UPROPERTY(EditAnywhere, Category = "Anim")
	int32 AnimPoseType;

	UPROPERTY(EditAnywhere, Category = "Anim")
	int32 AnimIndex;

// 넉백 방향
protected:
	FVector ImpulseDirection;


// Flag
protected:
	bool bIsDead{ false };
};
