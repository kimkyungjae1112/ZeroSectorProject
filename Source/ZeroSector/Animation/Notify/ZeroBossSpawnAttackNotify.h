// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ZeroBossSpawnAttackNotify.generated.h"

class AZeroCharacterMeleeZombie;

UCLASS()
class ZEROSECTOR_API UZeroBossSpawnAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UZeroBossSpawnAttackNotify();

	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Zero")
	TArray<TSubclassOf<AZeroCharacterMeleeZombie>> CommonMeleeZombieClass;

	UPROPERTY(EditDefaultsOnly, Category = "Zero")
	TSubclassOf<AZeroCharacterMeleeZombie> MiniMeleeZombieClass;

	UPROPERTY(EditDefaultsOnly, Category = "Zero")
	TArray<TSubclassOf<AZeroCharacterMeleeZombie>> TankerMeleeZombieClass;
};
