#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroWeaponAnimDataTable.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FZeroWeaponAnimDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> ReloadingMontage;

};