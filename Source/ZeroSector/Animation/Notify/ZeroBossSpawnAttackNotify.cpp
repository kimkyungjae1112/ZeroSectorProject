// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Notify/ZeroBossSpawnAttackNotify.h"
#include "Character/Zombie/ZeroCharacterMeleeZombie.h"
#include "GameFramework/Character.h"

void UZeroBossSpawnAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Character) return;
		
	UWorld* World = Character->GetWorld();
	if (!World) return;

	int32 RandIndex = FMath::RandRange(0, 3);
	int32 Index = MeleeZombieClass.IsValidIndex(RandIndex) ? RandIndex : 0;
	FVector SpawnLoc = Character->GetMesh()->GetSocketLocation(TEXT("hand_lSocket_Spawn")); 
	AZeroCharacterMeleeZombie* Zombie = World->SpawnActor<AZeroCharacterMeleeZombie>(MeleeZombieClass[Index], SpawnLoc, FRotator());
	if (Zombie)
	{
		Zombie->SpawnInit(Character->GetActorForwardVector());
	}
}

UZeroBossSpawnAttackNotify::UZeroBossSpawnAttackNotify()
{
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> MeleeZombieClassRef1(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieCommon.BP_MeleeZombieCommon_C"));
	if (MeleeZombieClassRef1.Class)
	{
		MeleeZombieClass.Add(MeleeZombieClassRef1.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> MeleeZombieClassRef2(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieMini.BP_MeleeZombieMini_C"));
	if (MeleeZombieClassRef2.Class)
	{
		MeleeZombieClass.Add(MeleeZombieClassRef2.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> MeleeZombieClassRef3(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieTanker.BP_MeleeZombieTanker_C"));
	if (MeleeZombieClassRef3.Class)
	{
		MeleeZombieClass.Add(MeleeZombieClassRef3.Class);
	}
}