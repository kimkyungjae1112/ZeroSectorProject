// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Zombie/ZeroCharacterBossZombie.h"
#include "Component/ZeroStatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Game/ZeroGameModeBase.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Engine/DamageEvents.h"
#include "Perception/AISense_Damage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
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
	return 500.f;
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

void AZeroCharacterBossZombie::SetAISpawnAttackDelegate(const FOnSpawnAttackFinished& InOnSpawnAttackFinished)
{
	OnSpawnAttackFinished = InOnSpawnAttackFinished;
}

void AZeroCharacterBossZombie::AttackOneByAI()
{
	Super::AttackOneByAI();

	BeginAttackOne();
}

void AZeroCharacterBossZombie::AttackTwoByAI()
{
	Super::AttackOneByAI();

	BeginRushAttack();
}

// Not Super
void AZeroCharacterBossZombie::SpawnAttackByAI()
{
	BeginSpawnAttack();
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
	ensure(Anim);
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

void AZeroCharacterBossZombie::BeginRushAttack()
{
	Anim->Montage_Play(RushAttackMontage);

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->BossRushAttackSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->BossRushAttackSFX);
	}

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterBossZombie::EndRushAttack);
	Anim->Montage_SetEndDelegate(MontageEnd, RushAttackMontage);
}

void AZeroCharacterBossZombie::EndRushAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnAttackTwoFinished.ExecuteIfBound();
}

void AZeroCharacterBossZombie::BeginSpawnAttack()
{
	Anim->Montage_Play(SpawnAttackMontage);

	UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetWorld()->GetGameInstance());
	if (GI && GI->GetSoundManager() && GI->GetSoundManager()->BossSpawnAttackSFX)
	{
		UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->BossSpawnAttackSFX);
	}

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &AZeroCharacterBossZombie::EndSpawnAttack);
	Anim->Montage_SetEndDelegate(MontageEnd, SpawnAttackMontage);
}

void AZeroCharacterBossZombie::EndSpawnAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	OnSpawnAttackFinished.ExecuteIfBound();
}

void AZeroCharacterBossZombie::BeginDead()
{
	if (bIsDead) return;
	bIsDead = true;
	
	Anim->Montage_Play(DeadMontage);

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

void AZeroCharacterBossZombie::ShowDangerDecal(FVector AttackLocation, float Radius, float Duration)
{
	DangerDecal = NewObject<UDecalComponent>(this);
	if (!DangerDecal) return;

	DangerDecal->RegisterComponent();
	DangerDecal->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	DangerDecal->SetWorldLocation(AttackLocation + FVector(0, 0, 10.f)); // 살짝 띄워서 Z-fighting 방지
	DangerDecal->SetDecalMaterial(DangerMaterial);
	DangerDecal->DecalSize = FVector(Radius, Radius, Radius);
	DangerDecal->SetFadeScreenSize(0.001f); // 너무 멀면 안 보이게

	// 페이드 아웃 (디칼 제거)
	DangerDecal->SetFadeOut(Duration, 1.0f, false); // (지속 시간, 페이드시간, 동기화 여부)
}
