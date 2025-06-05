// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterRangedZombie.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ZeroGameModeBase.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "ZeroRangedZombieProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
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

	ScheduleNextMove();
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

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ZombieDieSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->ZombieDieSFX);
	}

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

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AZeroCharacterRangedZombie::BeginHitReaction()
{
	UZeroAnimInstanceZombie* ZombieAnim = Cast<UZeroAnimInstanceZombie>(Anim);
	if (ZombieAnim)
	{
		ZombieAnim->bIsHit = true;
	}

	GetCharacterMovement()->MaxWalkSpeed = 50.f;
	Anim->Montage_Play(GetHitReactionMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterRangedZombie::EndHitReaction);
	Anim->Montage_SetEndDelegate(MontageEnd, GetHitReactionMontage());
}

void AZeroCharacterRangedZombie::EndHitReaction(UAnimMontage* Target, bool IsProperlyEnded)
{
	UZeroAnimInstanceZombie* ZombieAnim = Cast<UZeroAnimInstanceZombie>(Anim);
	if (ZombieAnim)
	{
		ZombieAnim->bIsHit = false;
	}

	GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
}

void AZeroCharacterRangedZombie::BeginStagger()
{
	Anim->Montage_Play(GetStaggerMontage());
}

UAnimMontage* AZeroCharacterRangedZombie::GetHitReactionMontage() const
{
	if (ZeroZombieAnimDataTable.HitReactionMontages[AnimPoseType].HitReactionMontage[AnimIndex].IsPending())
	{
		ZeroZombieAnimDataTable.HitReactionMontages[AnimPoseType].HitReactionMontage[AnimIndex].LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.HitReactionMontages[AnimPoseType].HitReactionMontage[AnimIndex].Get();
}

UAnimMontage* AZeroCharacterRangedZombie::GetStaggerMontage() const
{
	if (ZeroZombieAnimDataTable.StaggerMontages[AnimPoseType].IsPending())
	{
		ZeroZombieAnimDataTable.StaggerMontages[AnimPoseType].LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.StaggerMontages[AnimPoseType].Get();
}

void AZeroCharacterRangedZombie::PlayZombieMove()
{
	if (GetVelocity().Size() > 3.f)
	{
		UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
		if (GI && GI->GetSoundManager() && GI->GetSoundManager()->ZombieMoveSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GI->GetSoundManager()->ZombieMoveSFX, GetActorLocation());
		}
	}

	ScheduleNextMove();
}

void AZeroCharacterRangedZombie::ScheduleNextMove()
{
	float NextTime = FMath::RandRange(3.0f, 7.0f);
	GetWorld()->GetTimerManager().SetTimer(
		ZombieGrowlTimer,
		this,
		&AZeroCharacterRangedZombie::PlayZombieMove,
		NextTime,
		false
	);
}
