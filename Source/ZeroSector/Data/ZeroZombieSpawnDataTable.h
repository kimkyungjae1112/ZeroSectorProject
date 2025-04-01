
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroZombieSpawnDataTable.generated.h"

USTRUCT(BlueprintType)
struct FZeroZombieSpawnDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	uint8 MaxWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TArray<uint8> ZombieNum;
};