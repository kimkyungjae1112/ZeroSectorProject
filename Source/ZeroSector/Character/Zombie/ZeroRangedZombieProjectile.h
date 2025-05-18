// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZeroRangedZombieProjectile.generated.h"

class UProjectileMovementComponent;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class ZEROSECTOR_API AZeroRangedZombieProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AZeroRangedZombieProjectile();

	void InitDirection(const FVector& Direction);

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ProjectileOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UNiagaraComponent> NiagaraComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UProjectileMovementComponent> ProjectileComp;

};
