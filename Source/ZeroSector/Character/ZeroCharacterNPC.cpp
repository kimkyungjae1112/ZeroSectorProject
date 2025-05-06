// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterNPC.h"
#include "Component/ZeroDialogueComponent.h"
#include "Component/ZeroNPCWidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "UI/ZeroNPCNameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ZeroGameModeBase.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZeroSector.h"

AZeroCharacterNPC::AZeroCharacterNPC()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NPC"));
	
	DialogueComp = CreateDefaultSubobject<UZeroDialogueComponent>(TEXT("Dialogue Component"));

	WidgetComp = CreateDefaultSubobject<UZeroNPCWidgetComponent>(TEXT("Widget Component"));
	WidgetComp->SetupAttachment(GetMesh());
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 195.f));
	static ConstructorHelpers::FClassFinder<UZeroNPCNameWidget> NPCNameClasRef(TEXT("/Game/Blueprints/UI/WBP_NPCName.WBP_NPCName_C"));
	if (NPCNameClasRef.Class)
	{
		NPCNameClass = NPCNameClasRef.Class;
		WidgetComp->SetWidgetClass(NPCNameClass);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		WidgetComp->SetDrawSize(FVector2D(150.f, 15.f));
		WidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WidgetComp->SetVisibility(false);
	}

	TeamId = FGenericTeamId(0);
	ClassName = TEXT("NPC");
}

void AZeroCharacterNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode->GetDaySequence() == EDaySequence::EAfternoon)
	{
		DisplayName();
	}
}

void AZeroCharacterNPC::BeginPlay()
{
	Super::BeginPlay();
}

void AZeroCharacterNPC::DisplayName()
{
	FVector Origin = GetActorLocation();
	FCollisionQueryParams Params(NAME_None, false, this);
	TArray<FOverlapResult> Results;

	bool bResult = GetWorld()->OverlapMultiByChannel(Results, Origin, FQuat(), ECC_GameTraceChannel2, FCollisionShape::MakeSphere(1500.f), Params);
	if (bResult)
	{
		for (const auto& Result : Results)
		{
			if (Result.GetActor()->ActorHasTag(TEXT("Player")))
			{
				float Dist = FVector::Dist(Result.GetActor()->GetActorLocation(), GetActorLocation());
				if (Dist <= 600.f)
				{
					WidgetComp->SetVisibility(true);
				}
				else
				{
					WidgetComp->SetVisibility(false);
				}
			}
		}
	}
}
