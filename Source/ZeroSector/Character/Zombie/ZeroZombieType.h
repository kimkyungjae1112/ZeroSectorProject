
#pragma once

#include "CoreMinimal.h"
#include "ZeroZombieType.generated.h"

UENUM()
enum class EZombieType : uint8
{
	EZ_Common,
	EZ_Tanker,
	EZ_Mini,
	EZ_Ranged,
	EZ_Boss
};