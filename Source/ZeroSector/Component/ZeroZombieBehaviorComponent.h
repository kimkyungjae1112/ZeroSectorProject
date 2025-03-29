// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Zombie/ZeroZombieAnimDataTable.h"
#include "Character/Zombie/ZeroZombieType.h"
#include "ZeroZombieBehaviorComponent.generated.h"

class UAnimInstance;
class UAnimMontage;

DECLARE_DELEGATE(FNotifyFinishedAttackOne)
DECLARE_DELEGATE(FNotifyFinishedAttackTwo)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROSECTOR_API UZeroZombieBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZeroZombieBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	void BeginAttackOne();
	void EndOneAttack(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginAttackTwo();
	void EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded);

	void BeginDead();

public:
	FNotifyFinishedAttackOne NotifyFinishedAttackOne;
	FNotifyFinishedAttackTwo NotifyFinishedAttackTwo;

private:
	class UAnimMontage* GetAttackOneMontage() const;
	class UAnimMontage* GetAttackTwoMontage() const;
	class UAnimMontage* GetDeadMontage() const;

private:
	UPROPERTY()
	TObjectPtr<ACharacter> Character;

	UPROPERTY()
	TObjectPtr<UAnimInstance> Anim;

	UPROPERTY()
	FZeroZombieAnimDataTable ZeroZombieAnimDataTable;
	
	UPROPERTY()
	TObjectPtr<UDataTable> DataTableBuffer;
};
