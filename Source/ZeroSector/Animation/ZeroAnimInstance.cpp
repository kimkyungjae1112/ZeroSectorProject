// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ZeroAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UZeroAnimInstance::UZeroAnimInstance()
{
	MovingThreshold = 3.f;
}

void UZeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UZeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size();
		bIsIdle = GroundSpeed < MovingThreshold;
		Angle = Character->GetBaseAimRotation();
		bIsFalling = Movement->IsFalling();
	}
}
