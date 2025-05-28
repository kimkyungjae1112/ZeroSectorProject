// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroOperationBoard.h"
#include "Components/WidgetComponent.h"
#include "Component/ZeroPlayerStatComponent.h"
#include "Blueprint/UserWidget.h"
#include "Game/ZeroGameModeBase.h"

AZeroOperationBoard::AZeroOperationBoard()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(TEXT("/Script/Engine.StaticMesh'/Game/FC_MilitaryCamp/Models/MilitaryModels/SM_GeneratorPanel.SM_GeneratorPanel'"));
	if (MeshRef.Object)
	{
		MeshComp->SetStaticMesh(MeshRef.Object);
	}

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	WidgetComp->SetupAttachment(MeshComp);
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> OpBoardLocRef(TEXT("/Game/Blueprints/UI/WBP_OpBoardLoc.WBP_OpBoardLoc_C"));
	if (OpBoardLocRef.Class)
	{
		WidgetComp->SetWidgetClass(OpBoardLocRef.Class);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComp->SetDrawSize(FVector2D(150.f, 100.f));
		WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WidgetComp->SetVisibility(false);
	}

	Tags.Add(TEXT("OperationBoard"));
}

void AZeroOperationBoard::BeginPlay()
{
	Super::BeginPlay();
	
	UZeroPlayerStatComponent::OnZeroActivePoint.BindUObject(this, &AZeroOperationBoard::SetUIDisplay);
}

void AZeroOperationBoard::AccordingToDayOpBoardSelect() const
{
	if (StartDay == AZeroGameModeBase::Day)
	{
		WidgetComp->SetVisibility(true);
	}
}

void AZeroOperationBoard::SetUIDisplay()
{
	AccordingToDayOpBoardSelect();
}


