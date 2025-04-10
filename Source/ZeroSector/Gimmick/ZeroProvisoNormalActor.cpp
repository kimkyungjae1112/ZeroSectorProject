// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroProvisoNormalActor.h"
#include "Interface/ZeroOutlineInterface.h"

AZeroProvisoNormalActor::AZeroProvisoNormalActor()
{
	Tags.Add(TEXT("Proviso"));
}

void AZeroProvisoNormalActor::BeginPlay()
{
	Super::BeginPlay();

	IZeroOutlineInterface* OutlineInterface = Cast<IZeroOutlineInterface>(this);
	if (OutlineInterface)
	{
		OutlineInterface->SetOverlayMaterial();
	}
}
