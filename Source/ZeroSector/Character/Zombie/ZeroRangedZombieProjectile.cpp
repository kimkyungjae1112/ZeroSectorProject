// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroRangedZombieProjectile.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AZeroRangedZombieProjectile::AZeroRangedZombieProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AZeroRangedZombieProjectile::ProjectileOnHit);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp->SetupAttachment(CollisionComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileComp->UpdatedComponent = CollisionComp;
	ProjectileComp->InitialSpeed = 1500.f;
	ProjectileComp->MaxSpeed = 1500.f;
	ProjectileComp->bAutoActivate = false;
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->bShouldBounce = true;
}

void AZeroRangedZombieProjectile::InitDirection(const FVector& Direction)
{
	ProjectileComp->Velocity = Direction * ProjectileComp->InitialSpeed;
	ProjectileComp->Activate();
}

void AZeroRangedZombieProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeroRangedZombieProjectile::ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{
			// 터지는 이펙트 및 대미지 적용

			Destroy();
			return;
		}
		
		FTimerHandle BoomTimer;
		GetWorld()->GetTimerManager().SetTimer(BoomTimer, [&]()
			{
				// 터지는 이펙트 및 대미지 적용
				Destroy();
			}, 3.f, false);
	}
}

