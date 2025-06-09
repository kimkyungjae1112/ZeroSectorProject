// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroZombieSpawner.h"
#include "Components/SplineComponent.h"
#include "Character/Zombie/ZeroCharacterMeleeZombie.h"
#include "Character/Zombie/ZeroCharacterRangedZombie.h"
#include "Character/Zombie/ZeroCharacterBossZombie.h"
#include "ZeroSector.h"

AZeroZombieSpawner::AZeroZombieSpawner()
{
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
	RootComponent = SplineComp;
}

void AZeroZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeroZombieSpawner::SpawnZombie(uint8 CommonZombieNum, uint8 RangedZombieNum, uint8 MiniZombieNum, uint8 TankerZombieNum, uint8 BossZombieNum)
{
    ZE_LOG(LogZeroSector, Display, TEXT("Spawn Zombie"));
    int32 SumOfZombieNum = CommonZombieNum + RangedZombieNum + MiniZombieNum + TankerZombieNum + BossZombieNum;
    const float SplineLength = SplineComp->GetSplineLength();
    const float Spacing = SplineLength / SumOfZombieNum;

    for (int32 i = 0; i < SumOfZombieNum; ++i)
    {
        float Distance = i * Spacing;
        if (Distance > SplineLength) break;

        FVector Location = SplineComp->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
        FRotator Rotation = SplineComp->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

        int32 Index = i;

        if (Index < CommonZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(CommonMeleeZombieClass[FMath::RandRange(0, 2)], Location, Rotation);
        }
        else if (Index < CommonZombieNum + RangedZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterRangedZombie>(RangedZombieClass, Location, Rotation);
        }
        else if (Index < CommonZombieNum + RangedZombieNum + MiniZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(MiniMeleeZombieClass, Location, Rotation);
        }
        else if (Index < CommonZombieNum + RangedZombieNum + MiniZombieNum + TankerZombieNum)
        {
            ZE_LOG(LogZeroSector, Display, TEXT("Tanker Zombie Spawn"));
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(TankerMeleeZombieClass[FMath::RandRange(0, 2)], Location, Rotation);
        }
        else
        {
            GetWorld()->SpawnActor<AZeroCharacterBossZombie>(BossZombieClass, Location, Rotation);
        }
    }
}


