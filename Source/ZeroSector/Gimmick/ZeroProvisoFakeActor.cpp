// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroProvisoFakeActor.h"
#include "ZeroProvisoFakeActor.h"

AZeroProvisoFakeActor::AZeroProvisoFakeActor()
{
	Tags.Add(TEXT("FakeProviso"));
}

void AZeroProvisoFakeActor::BeginPlay()
{
	Super::BeginPlay();
}
