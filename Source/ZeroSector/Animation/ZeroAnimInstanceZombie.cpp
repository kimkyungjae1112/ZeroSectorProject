// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ZeroAnimInstanceZombie.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UZeroAnimInstanceZombie::UZeroAnimInstanceZombie()
{
	MovingThreshold = 3.f;
}

void UZeroAnimInstanceZombie::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character)
	{
		Movement = Character->GetCharacterMovement();
	}
}

void UZeroAnimInstanceZombie::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size();
		bIsIdle = GroundSpeed < MovingThreshold;
	}
}
