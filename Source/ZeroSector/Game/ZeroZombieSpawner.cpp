// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroZombieSpawner.h"
#include "Components/SplineComponent.h"
#include "Character/Zombie/ZeroCharacterMeleeZombie.h"

AZeroZombieSpawner::AZeroZombieSpawner()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComp;
}

void AZeroZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeroZombieSpawner::SpawnZombie(uint8 InZombieNum)
{
    const float SplineLength = SplineComp->GetSplineLength();

    for (int32 i = 0; i < InZombieNum; ++i)
    {
        float Distance = i * Spacing;
        if (Distance > SplineLength) break;

        FVector Location = SplineComp->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
        FRotator Rotation = SplineComp->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

        GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(MeleeZombieClass[static_cast<int>(FMath::RandRange(0.0, 2.0))], Location, Rotation);
    }
}


