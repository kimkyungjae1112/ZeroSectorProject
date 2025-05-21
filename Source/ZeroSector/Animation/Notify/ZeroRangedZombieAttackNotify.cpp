// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/ZeroRangedZombieAttackNotify.h"
#include "Interface/ZeroRangedZombieAttackInterface.h"

void UZeroRangedZombieAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IZeroRangedZombieAttackInterface* Interface = Cast<IZeroRangedZombieAttackInterface>(MeshComp->GetOwner());
	if (Interface)
	{
		Interface->SpawnProjectile();
	}
}

UZeroRangedZombieAttackNotify::UZeroRangedZombieAttackNotify()
{
}
