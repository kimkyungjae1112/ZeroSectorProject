// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZeroAnimInstanceZombie.generated.h"

class ACharacter;
class UCharacterMovementComponent;

UCLASS()
class ZEROSECTOR_API UZeroAnimInstanceZombie : public UAnimInstance
{
	GENERATED_BODY()

public:
	UZeroAnimInstanceZombie();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	uint8 bIsPose06 : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	FVector Velocity;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	int32 AnimPoseType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	int32 AnimIndex;
};