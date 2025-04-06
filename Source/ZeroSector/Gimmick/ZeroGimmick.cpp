// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroGimmick.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstance.h"
#include "ZeroSector.h"

AZeroGimmick::AZeroGimmick()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> OutlineMaterialRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Material/OutlineMaterial_Inst.OutlineMaterial_Inst'"));
	if (OutlineMaterialRef.Object)
	{
		OutlineMaterial = OutlineMaterialRef.Object;
	}

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName(TEXT("Interact"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetCollisionProfileName(TEXT("NoCollision"));
}

void AZeroGimmick::SetOverlayMaterial()
{
	MeshComp->SetOverlayMaterial(OutlineMaterial);
}

void AZeroGimmick::SetUnOverlayMaterial()
{
	MeshComp->SetOverlayMaterial(nullptr);
}

void AZeroGimmick::BeginPlay()
{
	Super::BeginPlay();
	
}


