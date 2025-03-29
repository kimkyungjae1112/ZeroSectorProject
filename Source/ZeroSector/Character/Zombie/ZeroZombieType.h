
#pragma once

#include "CoreMinimal.h"
#include "ZeroZombieType.generated.h"

UENUM()
enum class EZombieType : uint8
{
	EZ_Common,
	EZ_Tanker,
	EZ_Speed,
	EZ_Ranged,
	EZ_Boss
};