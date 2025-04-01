// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/ZeroCharacterStat.h"
#include "ZeroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZero)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROSECTOR_API UZeroStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZeroStatComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnHpZero OnHpZero;
	FOnHpChanged OnHpChanged;

	FORCEINLINE void SetModifierStat(const FZeroCharacterStat& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE FZeroCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	float ApplyDamage(float InDamage);

protected:
	void SetHp(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly)
	FName CurrentName;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	FZeroCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = "true"))
	FZeroCharacterStat ModifierStat;

	UPROPERTY()
	TObjectPtr<UDataTable> DataTableBuffer;
};
