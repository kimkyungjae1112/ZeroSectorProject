// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterMeleeZombie.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Game/ZeroGameModeBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
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

	StatComp->ApplyDamage(Damage);

	return 0.0f;
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

UAnimMontage* AZeroCharacterMeleeZombie::GetDeadMontage() const
{
	/*if (ZeroZombieAnimDataTable.DeadMontage.IsPending())
	{
		ZeroZombieAnimDataTable.DeadMontage.LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.DeadMontage.Get();*/
	return nullptr;
}
