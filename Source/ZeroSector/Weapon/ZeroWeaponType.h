
#pragma once

#include "CoreMinimal.h"
#include "ZeroWeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ENone = 0,
	EPistol,
	ERifle,
	EShotgun
};