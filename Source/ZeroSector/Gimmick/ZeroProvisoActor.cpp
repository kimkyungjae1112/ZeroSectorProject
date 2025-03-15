// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroProvisoActor.h"

AZeroProvisoActor::AZeroProvisoActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Tags.Add(TEXT("Proviso"));
}

void AZeroProvisoActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZeroProvisoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


