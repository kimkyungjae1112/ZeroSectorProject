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
#include "AI/Controller/ZeroAIControllerNPC.h"
#include "Player/ZeroPlayerController.h"
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

	InterviewWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interview Widget Component"));
	InterviewWidgetComp->SetupAttachment(GetMesh());
	InterviewWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 250.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> InterviewNPCLocRef(TEXT("/Game/Blueprints/UI/WBP_OpBoardLoc.WBP_OpBoardLoc_C"));
	if (InterviewNPCLocRef.Class)
	{
		InterviewWidgetComp->SetWidgetClass(InterviewNPCLocRef.Class);
		InterviewWidgetComp->SetWidgetSpace(EWidgetSpace::World);
		InterviewWidgetComp->SetDrawSize(FVector2D(100.f, 100.f));
		InterviewWidgetComp->SetPivot(FVector2D(0.5f, 0.5f));
		InterviewWidgetComp->SetTwoSided(true);
		InterviewWidgetComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InterviewWidgetComp->SetVisibility(false);
	}

	TeamId = FGenericTeamId(0);
	ClassName = TEXT("NPC");
}

void AZeroCharacterNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InterviewWidgetComp->IsVisible())
	{
		WidgetYawRotation += 150.0f * DeltaTime;

		if (WidgetYawRotation > 360.0f)
			WidgetYawRotation -= 360.0f;

		FRotator NewRotation(0.f, WidgetYawRotation, 0.f);
		InterviewWidgetComp->SetRelativeRotation(NewRotation);
	}
	else
	{
		AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode->GetDaySequence() == EDaySequence::EAfternoon)
		{
			DisplayName();
		}
	}
}

void AZeroCharacterNPC::DisplayInterviewMark(FString Name)
{
	if (Name == DisplayNPCName)
	{
		ZE_LOG(LogZeroSector, Display, TEXT("Display interview Mark"));
		InterviewWidgetComp->SetVisibility(true);
	}
}

void AZeroCharacterNPC::UnDisplayInterviewMark(FString Name)
{
	if (Name == DisplayNPCName)
	{
		ZE_LOG(LogZeroSector, Display, TEXT("Display interview UnMark"));
		InterviewWidgetComp->SetVisibility(false);
	}
}

void AZeroCharacterNPC::BeginPlay()
{
	Super::BeginPlay();

	AZeroGameModeBase* GameMode = CastChecked<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnAfternoonLocation.AddUObject(this, &AZeroCharacterNPC::MoveNextDayStartLoc);
	}

	if (ClassName == TEXT("Speedwagon"))
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	else
	{
		StartRandomPatrolPause();
	}

	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->OnStartInterview.AddUObject(this, &AZeroCharacterNPC::DisplayInterviewMark);
		PC->OnEndInterview.AddUObject(this, &AZeroCharacterNPC::UnDisplayInterviewMark);
	}
}

AZeroAIControllerNPC* AZeroCharacterNPC::GetAIController() const
{
	return CastChecked<AZeroAIControllerNPC>(GetController());
}

void AZeroCharacterNPC::MoveNextDayStartLoc(const FVector& InLocation)
{
	SetActorLocation(InLocation + FVector(300.f, 0.f, 0.f));

	GetAIController()->MoveToNextPoint();
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

void AZeroCharacterNPC::StartRandomPatrolPause()
{
	float PauseTime = FMath::RandRange(3.f, 5.f);

	GetWorld()->GetTimerManager().SetTimer(PatrolPauseTimer, this, &AZeroCharacterNPC::PauseMovement, PauseTime, false);
}

void AZeroCharacterNPC::PauseMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	float ResumeTime = FMath::RandRange(1.5f, 3.f);
	GetWorld()->GetTimerManager().SetTimer(PatrolResumeTimer, this, &AZeroCharacterNPC::ResumeMovement, ResumeTime, false);

}

void AZeroCharacterNPC::ResumeMovement()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	StartRandomPatrolPause();
}
