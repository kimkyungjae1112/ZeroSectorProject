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
    uint8 SumOfZombieNum = CommonZombieNum + RangedZombieNum + MiniZombieNum + TankerZombieNum + BossZombieNum;
    const float SplineLength = SplineComp->GetSplineLength();
    const float Spacing = SplineLength / SumOfZombieNum;

    for (int32 i = 0; i < SumOfZombieNum; ++i)
    {
        float Distance = i * Spacing;
        if (Distance > SplineLength) break;

        FVector Location = SplineComp->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
        FRotator Rotation = SplineComp->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

        if (i < CommonZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(MeleeZombieClass[0], Location, Rotation);
            continue;
        }
        else if (i > CommonZombieNum && i < CommonZombieNum + RangedZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(RangedZombieClass, Location, Rotation);
            continue;
        }
        else if (i > CommonZombieNum + RangedZombieNum && i < CommonZombieNum + RangedZombieNum + MiniZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(MeleeZombieClass[1], Location, Rotation);
            continue;
        }
        else if (i > CommonZombieNum + RangedZombieNum + MiniZombieNum && i < CommonZombieNum + RangedZombieNum + MiniZombieNum + TankerZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(MeleeZombieClass[2], Location, Rotation);
            continue;
        }
        else if (i > CommonZombieNum + RangedZombieNum + MiniZombieNum + TankerZombieNum)
        {
            GetWorld()->SpawnActor<AZeroCharacterMeleeZombie>(BossZombieClass, Location, Rotation);
            continue;
        }
    }
}


