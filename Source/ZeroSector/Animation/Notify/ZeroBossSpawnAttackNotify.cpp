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

	int32 RandIndex = FMath::RandRange(0, 2);
	FVector SpawnLoc = Character->GetMesh()->GetSocketLocation(TEXT("hand_lSocket_Spawn"));
	AZeroCharacterMeleeZombie* Zombie{ nullptr };

	switch (RandIndex)
	{
	case 0:
		Zombie = World->SpawnActor<AZeroCharacterMeleeZombie>(CommonMeleeZombieClass[FMath::RandRange(0, 2)], SpawnLoc, FRotator());
		break;
	case 1:
		Zombie = World->SpawnActor<AZeroCharacterMeleeZombie>(MiniMeleeZombieClass, SpawnLoc, FRotator());
		break;
	case 2:
		Zombie = World->SpawnActor<AZeroCharacterMeleeZombie>(TankerMeleeZombieClass[FMath::RandRange(0, 2)], SpawnLoc, FRotator());
		break;
	default:
		break;
	}

	if (Zombie)
	{
		Zombie->SpawnInit(Character->GetActorForwardVector());
	}
}

UZeroBossSpawnAttackNotify::UZeroBossSpawnAttackNotify()
{
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> CommonMeleeZombieClassRef(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieCommon.BP_MeleeZombieCommon_C"));
	if (CommonMeleeZombieClassRef.Class)
	{
		CommonMeleeZombieClass.Add(CommonMeleeZombieClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> CommonMeleeZombieClassRef1(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieCommon1.BP_MeleeZombieCommon1_C"));
	if (CommonMeleeZombieClassRef1.Class)
	{
		CommonMeleeZombieClass.Add(CommonMeleeZombieClassRef1.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> CommonMeleeZombieClassRef2(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieCommon2.BP_MeleeZombieCommon2_C"));
	if (CommonMeleeZombieClassRef2.Class)
	{
		CommonMeleeZombieClass.Add(CommonMeleeZombieClassRef2.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> MiniMeleeZombieClassRef(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieMini.BP_MeleeZombieMini_C"));
	if (MiniMeleeZombieClassRef.Class)
	{
		MiniMeleeZombieClass = MiniMeleeZombieClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> TankerMeleeZombieClassRef(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieTanker.BP_MeleeZombieTanker_C"));
	if (TankerMeleeZombieClassRef.Class)
	{
		TankerMeleeZombieClass.Add(TankerMeleeZombieClassRef.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> TankerMeleeZombieClassRef1(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieTanker1.BP_MeleeZombieTanker1_C"));
	if (TankerMeleeZombieClassRef1.Class)
	{
		TankerMeleeZombieClass.Add(TankerMeleeZombieClassRef1.Class);
	}
	static ConstructorHelpers::FClassFinder<AZeroCharacterMeleeZombie> TankerMeleeZombieClassRef2(TEXT("/Game/Blueprints/Character/Zombie/BP_MeleeZombieTanker2.BP_MeleeZombieTanker2_C"));
	if (TankerMeleeZombieClassRef2.Class)
	{
		TankerMeleeZombieClass.Add(TankerMeleeZombieClassRef2.Class);
	}
}