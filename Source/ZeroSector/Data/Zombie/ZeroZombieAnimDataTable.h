#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroZombieAnimDataTable.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FZeroZombieAnimDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> AttackOneMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> AttackTwoMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> DeadMontage;

};