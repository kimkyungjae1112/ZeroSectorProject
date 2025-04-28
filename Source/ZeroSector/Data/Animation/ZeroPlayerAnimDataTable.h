
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZeroPlayerAnimDataTable.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FPistolAnimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> ReloadingMontage;
};

USTRUCT(BlueprintType)
struct FRifleAnimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> ReloadingMontage;
};

USTRUCT(BlueprintType)
struct FShotgunAnimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> ReloadingMontage;
};

USTRUCT(BlueprintType)
struct FZeroPlayerAnimDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pistol")
	FPistolAnimData PistolAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rifle")
	FRifleAnimData RifleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shotgun")
	FShotgunAnimData ShotgunAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	TSoftObjectPtr<UAnimMontage> DeadMontage;

};