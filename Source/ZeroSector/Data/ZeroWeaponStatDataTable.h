
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroWeaponStatDataTable.generated.h"

USTRUCT(BlueprintType)
struct FZeroWeaponStatDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float MaxRange;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float RecoilRate;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 Magazine;
};