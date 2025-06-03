// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterMeleeZombie.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/ZeroAnimInstanceZombie.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "Game/ZeroGameModeBase.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroSector.h"

AZeroCharacterMeleeZombie::AZeroCharacterMeleeZombie()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> AnimDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/ZeroZombieAnimDataTable.ZeroZombieAnimDataTable'"));
	if (AnimDataTableRef.Succeeded())
	{
		DataTableBuffer = AnimDataTableRef.Object;
	}

	StatComp->OnHpZero.AddUObject(this, &AZeroCharacterMeleeZombie::BeginDead);

	TeamId = FGenericTeamId(1);
	ClassName = TEXT("Melee");
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Zombie"));
}

float AZeroCharacterMeleeZombie::GetAIAttackRange()
{
	return 500.0f;
}

float AZeroCharacterMeleeZombie::GetAITurnSpeed()
{
	return 2.0f;
}

float AZeroCharacterMeleeZombie::GetRunSpeed()
{
	return StatComp->GetTotalStat().RunSpeed;
}

float AZeroCharacterMeleeZombie::GetWalkSpeed()
{
	return StatComp->GetTotalStat().WalkSpeed;
}

void AZeroCharacterMeleeZombie::AttackOneByAI()
{
	Super::AttackOneByAI();

	BeginAttackOne();
}

void AZeroCharacterMeleeZombie::AttackTwoByAI()
{
	Super::AttackTwoByAI();

	BeginAttackTwo();
}

AController* AZeroCharacterMeleeZombie::GetAIController()
{
	return GetController();
}

FGenericTeamId AZeroCharacterMeleeZombie::GetGenericTeamId() const
{
	return TeamId;
}

float AZeroCharacterMeleeZombie::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
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

	int32 Probability = FMath::RandRange(1, 100);
	if (Probability > 0 && Probability <= 10)
	{
		BeginStagger();
	}
	else
	{
		BeginHitReaction();
	}
	StatComp->ApplyDamage(Damage);

	return 0.0f;
}

void AZeroCharacterMeleeZombie::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	FTimerHandle PhysicsTimer;
	GetWorld()->GetTimerManager().SetTimer(PhysicsTimer, [&]()
		{
			GetMesh()->SetSimulatePhysics(false);
			GetMesh()->SetCollisionProfileName(TEXT("Zombie"));

			FVector MeshLocation = GetMesh()->GetComponentLocation();
			FVector CapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
			FVector Delta = MeshLocation - CapsuleLocation;
			Delta.Z = 0; // Z축은 Capsule 기준으로 둠
			GetCapsuleComponent()->SetWorldLocation(MeshLocation);

			GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
			GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		}, 1.f, false);
}

void AZeroCharacterMeleeZombie::SpawnInit(const FVector& Direction)
{
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);

	const float ImpulseStrength = 5000.0f;
	FVector FinalImpulse = Direction * ImpulseStrength;

	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
	GetMesh()->AddImpulseToAllBodiesBelow(FinalImpulse, NAME_None);
}

void AZeroCharacterMeleeZombie::BeginPlay()
{
	Super::BeginPlay();

	ZeroZombieAnimDataTable = *DataTableBuffer->FindRow<FZeroZombieAnimDataTable>(GetClassName(), FString());
	
	Anim = GetMesh()->GetAnimInstance();
}

void AZeroCharacterMeleeZombie::BeginAttackOne()
{
	Anim->Montage_Play(GetAttackOneMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterMeleeZombie::EndAttackOne);
	Anim->Montage_SetEndDelegate(MontageEnd, GetAttackOneMontage());
}

void AZeroCharacterMeleeZombie::EndAttackOne(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnAttackOneFinished.ExecuteIfBound();
}

void AZeroCharacterMeleeZombie::BeginAttackTwo()
{
	Anim->Montage_Play(GetAttackTwoMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterMeleeZombie::EndAttackTwo);
	Anim->Montage_SetEndDelegate(MontageEnd, GetAttackTwoMontage());
}

void AZeroCharacterMeleeZombie::EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnAttackTwoFinished.ExecuteIfBound();
}

void AZeroCharacterMeleeZombie::BeginDead()
{
	if (bIsDead) return;
	bIsDead = true;
	ZE_LOG(LogZeroSector, Display, TEXT("BeginDead"));
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

void AZeroCharacterMeleeZombie::BeginHitReaction()
{
	UZeroAnimInstanceZombie* ZombieAnim = Cast<UZeroAnimInstanceZombie>(Anim);
	if (ZombieAnim)
	{
		ZombieAnim->bIsHit = true;
	}

	GetCharacterMovement()->MaxWalkSpeed = 50.f;
	Anim->Montage_Play(GetHitReactionMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterMeleeZombie::EndHitReaction);
	Anim->Montage_SetEndDelegate(MontageEnd, GetHitReactionMontage());
}

void AZeroCharacterMeleeZombie::EndHitReaction(UAnimMontage* Target, bool IsProperlyEnded)
{
	UZeroAnimInstanceZombie* ZombieAnim = Cast<UZeroAnimInstanceZombie>(Anim);
	if (ZombieAnim)
	{
		ZombieAnim->bIsHit = false;
	}

	GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
}

void AZeroCharacterMeleeZombie::BeginStagger()
{
	Anim->Montage_Play(GetStaggerMontage());
}

UAnimMontage* AZeroCharacterMeleeZombie::GetAttackOneMontage() const
{
	if (ZeroZombieAnimDataTable.AttackOneMontages[AnimPoseType].AttackOneMontage[AnimIndex].IsPending())
	{
		ZeroZombieAnimDataTable.AttackOneMontages[AnimPoseType].AttackOneMontage[AnimIndex].LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.AttackOneMontages[AnimPoseType].AttackOneMontage[AnimIndex].Get();
}

UAnimMontage* AZeroCharacterMeleeZombie::GetAttackTwoMontage() const
{
	if (ZeroZombieAnimDataTable.AttackTwoMontages[AnimPoseType].AttackTwoMontage[AnimIndex].IsPending())
	{
		ZeroZombieAnimDataTable.AttackTwoMontages[AnimPoseType].AttackTwoMontage[AnimIndex].LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.AttackTwoMontages[AnimPoseType].AttackTwoMontage[AnimIndex].Get();
}

UAnimMontage* AZeroCharacterMeleeZombie::GetHitReactionMontage() const
{
	if (ZeroZombieAnimDataTable.HitReactionMontages[AnimPoseType].HitReactionMontage[AnimIndex].IsPending())
	{
		ZeroZombieAnimDataTable.HitReactionMontages[AnimPoseType].HitReactionMontage[AnimIndex].LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.HitReactionMontages[AnimPoseType].HitReactionMontage[AnimIndex].Get();
}

UAnimMontage* AZeroCharacterMeleeZombie::GetStaggerMontage() const
{
	if (ZeroZombieAnimDataTable.StaggerMontages[AnimPoseType].IsPending())
	{
		ZeroZombieAnimDataTable.StaggerMontages[AnimPoseType].LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.StaggerMontages[AnimPoseType].Get();
}
