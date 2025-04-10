#pragma once

#include "CoreMinimal.h"
#include "ZeroProvisoType.generated.h" 

UENUM(BlueprintType)
enum class EZeroProvisoType : uint8
{
	Normal      UMETA(DisplayName = "Normal"),
	Hidden      UMETA(DisplayName = "Hidden"),
	Fake        UMETA(DisplayName = "Fake"),
};
