
#pragma once

#include "CoreMinimal.h"
#include "ZeroWeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EPistol = 0,
	ERifle,
	EShotgun
};