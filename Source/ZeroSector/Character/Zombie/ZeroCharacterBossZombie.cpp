// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterBossZombie.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ZeroGameModeBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroSector.h"

AZeroCharacterBossZombie::AZeroCharacterBossZombie()
{
	GetCharacterMovement()->MaxWalkSpeed = 250.f;

	StatComp->OnHpZero.AddUObject(this, &AZeroCharacterBossZombie::BeginDead);

	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Boss");
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));
}

float AZeroCharacterBossZombie::GetAIAttackRange()
{
	return 1000.f;
}

float AZeroCharacterBossZombie::GetAITurnSpeed()
{
	return 2.f;
}

float AZeroCharacterBossZombie::GetRunSpeed()
{
	return 250.0f;
}

float AZeroCharacterBossZombie::GetWalkSpeed()
{
	return 250.0f;
}

void AZeroCharacterBossZombie::AttackOneByAI()
{
	Super::AttackOneByAI();

	BeginAttackOne();
}

void AZeroCharacterBossZombie::AttackTwoByAI()
{
	Super::AttackOneByAI();
}

AController* AZeroCharacterBossZombie::GetAIController()
{
	return GetController();
}

FGenericTeamId AZeroCharacterBossZombie::GetGenericTeamId() const
{
	return TeamId;
}

float AZeroCharacterBossZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float SuperResult = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		ImpulseDirection = PointEvent->ShotDirection.GetSafeNormal();

		if (PointEvent)
		{
			UAISense_Damage::ReportDamageEvent(
				GetWorld(),
				this,
				DamageCauser,
				Damage,
				GetActorLocation(),
				PointEvent->ShotDirection.GetSafeNormal()
			);
		}
	}

	StatComp->ApplyDamage(Damage);

	return 0.0f;
}

void AZeroCharacterBossZombie::BeginPlay()
{
	Super::BeginPlay();

	Anim = GetMesh()->GetAnimInstance();
}

void AZeroCharacterBossZombie::BeginAttackOne()
{
	Anim->Montage_Play(AttackOneMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterBossZombie::EndAttackOne);
	Anim->Montage_SetEndDelegate(MontageEnd, AttackOneMontage);
}

void AZeroCharacterBossZombie::EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnAttackOneFinished.ExecuteIfBound();
}

void AZeroCharacterBossZombie::BeginAttackTwo()
{
}

void AZeroCharacterBossZombie::EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void AZeroCharacterBossZombie::BeginDead()
{
	// Ragdoll 보단 AnimMontage 로 하는게 나을듯 보스니까
	if (bIsDead) return;
	bIsDead = true;
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	const float ImpulseStrength = 1000.0f;
	FVector FinalImpulse = ImpulseDirection * ImpulseStrength;

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);

	DetachFromControllerPendingDestroy();
	ZE_LOG(LogZeroSector, Display, TEXT("Zombie Dead"));

	FTimerHandle DestoryTimer;
	GetWorld()->GetTimerManager().SetTimer(DestoryTimer, [&]()
		{
			Destroy();
		}, 5.f, false);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->PawnKilled(this);
	}
}
