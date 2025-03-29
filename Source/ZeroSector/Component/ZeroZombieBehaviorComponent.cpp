// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroZombieBehaviorComponent.h"
#include "Interface/ZeroClassIdentifierInterface.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "ZeroSector.h"

UZeroZombieBehaviorComponent::UZeroZombieBehaviorComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> AnimDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Zombie/ZeroZombieAnimDataTable.ZeroZombieAnimDataTable'"));
	if (AnimDataTableRef.Succeeded())
	{
		DataTableBuffer = AnimDataTableRef.Object;
	}
}

void UZeroZombieBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(GetOwner());
	if (CII)
	{
		FString ContextString(TEXT("AinmMontage Context"));
		ZeroZombieAnimDataTable = *DataTableBuffer->FindRow<FZeroZombieAnimDataTable>(CII->GetClassName(), ContextString);
	}

	Character = CastChecked<ACharacter>(GetOwner());
	Anim = Character->GetMesh()->GetAnimInstance();
	//check(Character && Anim);
}

void UZeroZombieBehaviorComponent::BeginAttackOne()
{
	Anim->Montage_Play(GetAttackOneMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UZeroZombieBehaviorComponent::EndOneAttack);
	Anim->Montage_SetEndDelegate(MontageEnd, GetAttackOneMontage());
}

void UZeroZombieBehaviorComponent::EndOneAttack(UAnimMontage* Target, bool IsProperlyEnded)
{
	NotifyFinishedAttackOne.ExecuteIfBound();
}

void UZeroZombieBehaviorComponent::BeginAttackTwo()
{
	Anim->Montage_Play(GetAttackTwoMontage());

	FOnMontageEnded MontageEnd;
	MontageEnd.BindUObject(this, &UZeroZombieBehaviorComponent::EndAttackTwo);
	Anim->Montage_SetEndDelegate(MontageEnd, GetAttackTwoMontage());
}

void UZeroZombieBehaviorComponent::EndAttackTwo(UAnimMontage* Target, bool IsProperlyEnded)
{
	NotifyFinishedAttackTwo.ExecuteIfBound();
}

void UZeroZombieBehaviorComponent::BeginDead()
{
	Anim->Montage_Play(GetDeadMontage());
}

UAnimMontage* UZeroZombieBehaviorComponent::GetAttackOneMontage() const
{
	if (ZeroZombieAnimDataTable.AttackOneMontage.IsPending())
	{
		ZeroZombieAnimDataTable.AttackOneMontage.LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.AttackOneMontage.Get();
}

UAnimMontage* UZeroZombieBehaviorComponent::GetAttackTwoMontage() const
{
	if (ZeroZombieAnimDataTable.AttackTwoMontage.IsPending())
	{
		ZeroZombieAnimDataTable.AttackTwoMontage.LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.AttackTwoMontage.Get();
}

UAnimMontage* UZeroZombieBehaviorComponent::GetDeadMontage() const
{
	if (ZeroZombieAnimDataTable.DeadMontage.IsPending())
	{
		ZeroZombieAnimDataTable.DeadMontage.LoadSynchronous();
	}
	return ZeroZombieAnimDataTable.DeadMontage.Get();
}
