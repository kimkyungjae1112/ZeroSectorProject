// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ZeroAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
		Angle = UKismetMathLibrary::InverseTransformDirection(Character->GetActorTransform(), Velocity);
		bIsFalling = Movement->IsFalling();
		if (Character->GetController())
		{
			ControlRotation = Character->GetController()->GetControlRotation();
			CalSpineRotation();
		}
	}
}

void UZeroAnimInstance::CalSpineRotation()
{
	float PitchRot = ControlRotation.Pitch;

	float Sub = 360.f - PitchRot;
	float Mul = PitchRot * -1.f;
	bool Greater = PitchRot > 180.f;

	float Selected = UKismetMathLibrary::SelectFloat(Sub, Mul, Greater) / 3.f;
	SpineRotation = UKismetMathLibrary::MakeRotator(Selected, 0.f, 0.f);
}
