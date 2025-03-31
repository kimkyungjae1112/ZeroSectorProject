// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroZombieSpawner.h"
#include "Components/SplineComponent.h"
#include "Character/Zombie/ZeroCharacterAIMeleeZombie.h"

AZeroZombieSpawner::AZeroZombieSpawner()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComp;
}

void AZeroZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
    // 임시
    SpawnZombie();
}

void AZeroZombieSpawner::SpawnZombie()
{
    const float SplineLength = SplineComp->GetSplineLength();

    for (int32 i = 0; i < ZombieNum; ++i)
    {
        float Distance = i * Spacing;
        if (Distance > SplineLength) break;

        FVector Location = SplineComp->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
        FRotator Rotation = SplineComp->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

        GetWorld()->SpawnActor<AZeroCharacterAIMeleeZombie>(MeleeZombieClass, Location, Rotation);
    }
}


