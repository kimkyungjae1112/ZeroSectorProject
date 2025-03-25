
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FZeroCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FZeroCharacterStat() : MaxHp(0.f), Attack(0.f), AttackRange(0.f), WalkSpeed(0.f), RunSpeed(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float RunSpeed;

	FZeroCharacterStat operator+(const FZeroCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FZeroCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FZeroCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; ++i)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};