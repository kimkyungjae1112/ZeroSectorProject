// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterRangedZombie.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ZeroGameModeBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "ZeroRangedZombieProjectile.h"
#include "ZeroSector.h"

AZeroCharacterRangedZombie::AZeroCharacterRangedZombie()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> AnimDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/ZeroZombieAnimDataTable.ZeroZombieAnimDataTable'"));
	if (AnimDataTableRef.Succeeded())
	{
		DataTableBuffer = AnimDataTableRef.Object;
	}

	StatComp->OnHpZero.AddUObject(this, &AZeroCharacterRangedZombie::BeginDead);

	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Ranged");
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));
}

FGenericTeamId AZeroCharacterRangedZombie::GetGenericTeamId() const
{
	return TeamId;
}

void AZeroCharacterRangedZombie::SpawnProjectile()
{
	FVector SpawnLoc = GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
	Projectile = GetWorld()->SpawnActor<AZeroRangedZombieProjectile>(ProjectileClass, SpawnLoc, FRotator());
	Projectile->SetOwner(this);
	Projectile->InitDirection(GetActorForwardVector());
	Projectile->InitController(GetController());
}

AController* AZeroCharacterRangedZombie::GetAIController()
{
	return GetController();
}

float AZeroCharacterRangedZombie::GetAIAttackRange()
{
	return 1500.0f;
}

float AZeroCharacterRangedZombie::GetAITurnSpeed()
{
	return 2.f;
}

float AZeroCharacterRangedZombie::GetRunSpeed()
{
	return StatComp->GetTotalStat().RunSpeed;
}

float AZeroCharacterRangedZombie::GetWalkSpeed()
{
	return StatComp->GetTotalStat().WalkSpeed;
}

void AZeroCharacterRangedZombie::AttackOneByAI()
{
	Super::AttackOneByAI();

	BeginAttackOne();
}

void AZeroCharacterRangedZombie::AttackTwoByAI()
{
	Super::AttackTwoByAI();
}

float AZeroCharacterRangedZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

void AZeroCharacterRangedZombie::BeginPlay()
{
	Super::BeginPlay();

	ZeroZombieAnimDataTable = *DataTableBuffer->FindRow<FZeroZombieAnimDataTable>(GetClassName(), FString());

	Anim = GetMesh()->GetAnimInstance();
}

AZeroAIControllerRangedZombie* AZeroCharacterRangedZombie::GetMyController() const
{
	return nullptr;
}

void AZeroCharacterRangedZombie::BeginAttackOne()
{
	Anim->Montage_Play(TempMontage);

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterRangedZombie::EndAttackOne);
	Anim->Montage_SetEndDelegate(MontageEnd, TempMontage);
}

void AZeroCharacterRangedZombie::EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnAttackOneFinished.ExecuteIfBound();
}

void AZeroCharacterRangedZombie::BeginAttackTwo()
{
}

void AZeroCharacterRangedZombie::EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded)
{
}

void AZeroCharacterRangedZombie::BeginDead()
{
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

UAnimMontage* AZeroCharacterRangedZombie::GetAttackOneMontage() const
{
	return nullptr;
}

UAnimMontage* AZeroCharacterRangedZombie::GetAttackTwoMontage() const
{
	return nullptr;
}
