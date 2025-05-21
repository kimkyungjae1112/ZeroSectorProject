
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Character/Zombie/ZeroZombieType.h"
#include "ZeroZombieSpawnDataTable.generated.h"

USTRUCT(BlueprintType)
struct FZeroZombieNum
{
	GENERATED_BODY()

public:
	FZeroZombieNum() {}

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<uint8> ZombieNum;
};

USTRUCT(BlueprintType)
struct FZeroZombieSpawnDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	uint8 MaxWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 MaxTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	TMap<EZombieType, FZeroZombieNum> ZombieNums;



};