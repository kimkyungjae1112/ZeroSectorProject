#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroZombieAnimDataTable.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FZeroAttackOneMontageArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UAnimMontage>> AttackOneMontage;
};

USTRUCT(BlueprintType)
struct FZeroAttackTwoMontageArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UAnimMontage>> AttackTwoMontage;
};

USTRUCT(BlueprintType)
struct FZeroHitReactionArray
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UAnimMontage>> HitReactionMontage;
};

USTRUCT(BlueprintType)
struct FZeroZombieAnimDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<FZeroAttackOneMontageArray> AttackOneMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<FZeroAttackTwoMontageArray> AttackTwoMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<FZeroHitReactionArray> HitReactionMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TArray<TSoftObjectPtr<UAnimMontage>> StaggerMontages;

};