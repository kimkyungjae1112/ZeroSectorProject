// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterNPC.h"
#include "Component/ZeroDialogueComponent.h"
#include "Components/CapsuleComponent.h"

AZeroCharacterNPC::AZeroCharacterNPC()
{
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NPC"));
	
	DialogueComp = CreateDefaultSubobject<UZeroDialogueComponent>(TEXT("Dialogue Component"));


	TeamId = FGenericTeamId(0);
	ClassName = TEXT("NPC");
}

void AZeroCharacterNPC::BeginPlay()
{
	Super::BeginPlay();
}
