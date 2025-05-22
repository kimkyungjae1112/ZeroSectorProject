// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroRangedZombieProjectile.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "ZeroSector.h"

AZeroRangedZombieProjectile::AZeroRangedZombieProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AZeroRangedZombieProjectile::ProjectileOnHit);
	CollisionComp->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = CollisionComp;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	NiagaraComp->SetupAttachment(CollisionComp);
	NiagaraComp->OnSystemFinished.AddDynamic(this, &AZeroRangedZombieProjectile::DestroyAfterExplosion);
	NiagaraComp->bAutoActivate = false;

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

void AZeroRangedZombieProjectile::InitController(AController* Controller)
{
	ZombieController = Controller;
}

void AZeroRangedZombieProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeroRangedZombieProjectile::ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != GetOwner() && !bIsHit)
	{
		bIsHit = true;
		if (OtherActor->ActorHasTag(TEXT("Player")))
		{
			// 터지는 이펙트 및 대미지 적용
			OtherActor->TakeDamage(100.f, FDamageEvent(), ZombieController, GetOwner());

			Destroy();
			return;
		}
		
		Explosion();
	}
}

void AZeroRangedZombieProjectile::Explosion()
{
	FTimerHandle BoomTimer;
	GetWorld()->GetTimerManager().SetTimer(BoomTimer, [&]()
		{
			UGameplayStatics::ApplyRadialDamage(
				GetWorld(),
				50.f,
				GetActorLocation(),
				500.f,
				TSubclassOf<UDamageType>(),
				TArray<AActor*>(),
				GetOwner(),
				ZombieController);
			NiagaraComp->Activate();
		}, 3.f, false);
}

void AZeroRangedZombieProjectile::DestroyAfterExplosion(UNiagaraComponent* PSystem)
{
	Destroy();
}
