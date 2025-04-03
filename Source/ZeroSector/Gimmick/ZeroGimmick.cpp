// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroGimmick.h"
#include "Components/BoxComponent.h"

AZeroGimmick::AZeroGimmick()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName(TEXT("Interact"));
}

void AZeroGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}


