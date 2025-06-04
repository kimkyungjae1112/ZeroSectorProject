// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroDialogueComponent.h"
#include "Data/ZeroSingleton.h"
#include "Data/ZeroDialogueOptionDataTable.h"
#include "Data/ZeroResearcherData.h"
#include "UI/ZeroDialogueWidget.h"
#include "UI/ZeroDialogueOptionWidget.h"
#include "UI/ZeroAfternoonHUDWidget.h"
#include "Components/ScrollBox.h"
#include "Player/ZeroPlayerController.h"
#include "Interface/ZeroClassIdentifierInterface.h"
#include "Game/ZeroGameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Controller/ZeroAIControllerNPC.h"
#include "ZeroSector.h"

UZeroDialogueComponent::UZeroDialogueComponent()
{
	static ConstructorHelpers::FClassFinder<UZeroDialogueWidget> DialogueWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_Dialogue.WBP_Dialogue_C"));
	if (DialogueWidgetClassRef.Class)
	{
		DialogueWidgetClass = DialogueWidgetClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<UZeroDialogueOptionWidget> DialogueOptionWidgetClassRef(TEXT("/Game/Blueprints/UI/WBP_DialogueOption.WBP_DialogueOption_C"));
	if (DialogueOptionWidgetClassRef.Class)
	{
		DialogueOptionWidgetClass = DialogueOptionWidgetClassRef.Class;
	}

	P_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_P.DialogueDataTable_P'"));
	S_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_S.DialogueDataTable_S'"));
	V_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_V.DialogueDataTable_V'"));
	C_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_C1.DialogueDataTable_C1'"));
	N1_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_N1.DialogueDataTable_N1'"));
	N2_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_N2.DialogueDataTable_N2'"));
	N3_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_N3.DialogueDataTable_N3'"));

	DialogueTableMap.Add(TEXT("데인 코너"), P_DialogueTable);
	DialogueTableMap.Add(TEXT("Speedwagon"), S_DialogueTable);
	DialogueTableMap.Add(TEXT("Vaccine"), V_DialogueTable);
	DialogueTableMap.Add(TEXT("Criminal"), C_DialogueTable);
	DialogueTableMap.Add(TEXT("Normal1"), N1_DialogueTable);
	DialogueTableMap.Add(TEXT("Normal2"), N2_DialogueTable);
	DialogueTableMap.Add(TEXT("Normal3"), N3_DialogueTable);

	V_Researcher = FSoftObjectPath(TEXT("/Script/ZeroSector.ZeroResearcherData'/Game/Data/Researcher/DA_Researcher_V.DA_Researcher_V'"));
	C_Researcher = FSoftObjectPath(TEXT("/Script/ZeroSector.ZeroResearcherData'/Game/Data/Researcher/DA_Researcher_C.DA_Researcher_C'"));
	N1_Researcher = FSoftObjectPath(TEXT("/Script/ZeroSector.ZeroResearcherData'/Game/Data/Researcher/DA_Researcher_N1.DA_Researcher_N1'"));
	N2_Researcher = FSoftObjectPath(TEXT("/Script/ZeroSector.ZeroResearcherData'/Game/Data/Researcher/DA_Researcher_N2.DA_Researcher_N2'"));
	N3_Researcher = FSoftObjectPath(TEXT("/Script/ZeroSector.ZeroResearcherData'/Game/Data/Researcher/DA_Researcher_N3.DA_Researcher_N3'"));

	ResearcherDataMap.Add(TEXT("Speedwagon"), V_Researcher);
	ResearcherDataMap.Add(TEXT("Vaccine"), V_Researcher);
	ResearcherDataMap.Add(TEXT("Criminal"), C_Researcher);
	ResearcherDataMap.Add(TEXT("Normal1"), N1_Researcher);
	ResearcherDataMap.Add(TEXT("Normal2"), N2_Researcher);
	ResearcherDataMap.Add(TEXT("Normal3"), N3_Researcher);

	PrevIndex = TEXT("1");
}

void UZeroDialogueComponent::StartDialogue()
{
	ZE_LOG(LogZeroSector, Display, TEXT("Start Dialogue"));
	
	InputModeUIOnly();

	if (DialogueTable.bIsOpenOption && bIsTalking)
	{
		for (const auto& DialogueOptionTable : DialogueTable.OptionDialogues)
		{
			DialogueOptionSpawn(DialogueOptionTable);
		}
		return;
	}

	if (bIsTalking)
	{
		InProgressDialogue();
		return;
	}

	RotationToPlayer();
	Character->GetCharacterMovement()->MaxWalkSpeed = 0;

	FString CurrentActorClassName;
	if (IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(GetOwner()))
	{
		CurrentActorClassName = CII->GetClassName().ToString();
	}

	AZeroPlayerController* PC = GetPlayerController();
	if (PC && PC->SelectedInterviewName == CurrentActorClassName && PC->GetAfternoonHUDWidget())
	{
		bIsInterview = true;
		PC->GetAfternoonHUDWidget()->HideInterviewText();
		StartInterviewDialogue(ReliabilityLevel);
	}

	
	ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueTable.Dialogue.ToString());
	bIsTalking = true;
	DialogueWidgetPtr = CreateWidget<UZeroDialogueWidget>(GetWorld(), DialogueWidgetClass);
	DialogueWidgetPtr->AddToViewport();
	DialogueWidgetPtr->SetDialogueText((DialogueTable.bIsPlayerDialogue ? FText::FromString(TEXT("데인 코너")) : ActorName), DialogueTable.Dialogue);

	if (DialogueTable.bIsOpenOption)
	{
		for (const auto& DialogueOptionTable : DialogueTable.OptionDialogues)
		{
			DialogueOptionSpawn(DialogueOptionTable);
		}
		return;
	}

	if(!bIsInterview) DialogueDataInit();
}

void UZeroDialogueComponent::SetupFinishedDialogueDelegate(const FOnFinishedDialogue& InOnFinishedDialogue)
{
	OnFinishedDialogue = InOnFinishedDialogue;
}

void UZeroDialogueComponent::ShutdownGame()
{
	ResearcherData->Trust = 0.f;
}

AZeroPlayerController* UZeroDialogueComponent::GetPlayerController() const
{
	return Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
}

void UZeroDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	if (IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(GetOwner()))
	{
		if (DialogueTableMap[CII->GetClassName()].IsPending())
		{
			DialogueTableMap[CII->GetClassName()].LoadSynchronous();
		}
		FString DialogueContext(TEXT("Dialogue Context"));
		DialogueTable = *DialogueTableMap[CII->GetClassName()]->FindRow<FZeroDialogueDataTable>(TEXT("1"), DialogueContext);
		PrevIndex = DialogueTable.PrevIndex;

		if (ResearcherDataMap[CII->GetClassName()] && ResearcherDataMap[CII->GetClassName()].IsPending())
		{
			ResearcherDataMap[CII->GetClassName()].LoadSynchronous();
		}
		ResearcherData = ResearcherDataMap[CII->GetClassName()].Get();
	}

	if (AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->OnStartAfternoon.AddUObject(this, &UZeroDialogueComponent::NextDayDialogue);
	}

	Character = Cast<ACharacter>(GetOwner());
	ensure(Character);
}

void UZeroDialogueComponent::OnClickedOption(FZeroDialogueDataTable InDialogueTable, float Reliability)
{
	ResearcherData->Trust += Reliability;

	// 임시 신뢰 단계
	if (ResearcherData->Trust < 100.f) ReliabilityLevel = 0;
	else if (ResearcherData->Trust >= 100.f && ResearcherData->Trust < 300.f) ReliabilityLevel = 1;
	else if (ResearcherData->Trust >= 300.f) ReliabilityLevel = 2;

	ZE_LOG(LogZeroSector, Display, TEXT("신뢰도 : %f, 신뢰 레벨 : %d"), ResearcherData->Trust, ReliabilityLevel);
	DialogueWidgetPtr->GetScrollBox()->ClearChildren();

	DialogueTable = InDialogueTable;
	ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueTable.Dialogue.ToString());

	DialogueWidgetPtr->SetDialogueText((DialogueTable.bIsPlayerDialogue ? FText::FromString(TEXT("데인 코너")) : ActorName), DialogueTable.Dialogue);
	if (DialogueTable.bIsOpenOption)
	{
		for (const auto& DialogueOptionTable : DialogueTable.OptionDialogues)
		{
			DialogueOptionSpawn(DialogueOptionTable);
		}
	}
	else if (DialogueTable.bIsEnd)
	{
		DialogueWidgetPtr->GetScrollBox()->ClearChildren();
		DialogueWidgetPtr->CloseNextDialogueButton();

		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
			{
				DialogueWidgetPtr->RemoveFromParent();
				OnFinishedDialogue.ExecuteIfBound();
				InputModeGameOnly();
				if (IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(Character))
				{
					if (CII && CII->GetClassName() == TEXT("Speedwagon"))
						Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
					else
						Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
				}
				GetAIController()->MoveToNextPoint();
			}, 3.f, false);


		FZeroDialogueDataTable* FoundRow = DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(PrevIndex, FString());
		DialogueTable = *FoundRow;
		
		bIsTalking = false;
		bIsInterview = false;
		GetPlayerController()->SelectedInterviewName = "";

		
	}
	else
	{
		DialogueWidgetPtr->DisplayNextDialogueButton();
		DialogueDataInit();
	}
}

void UZeroDialogueComponent::RotationToPlayer()
{
	ZE_LOG(LogZeroSector, Display, TEXT("플레이어로 회전"));
	APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (Character && Player)
	{
		FVector Dir = (Player->GetActorLocation() - Character->GetActorLocation()).GetSafeNormal();
		GetAIController()->StopMovement();
		Character->SetActorRotation(Dir.Rotation());
	}
}

void UZeroDialogueComponent::InputModeGameOnly()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->InputModeGameOnly();
	}
}

void UZeroDialogueComponent::InputModeGameAndUI()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->InputModeGameAndUI();
	}
}

void UZeroDialogueComponent::InputModeUIOnly()
{
	AZeroPlayerController* PC = Cast<AZeroPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->InputModeUIOnly();
	}
}

void UZeroDialogueComponent::InProgressDialogue()
{
	ZE_LOG(LogZeroSector, Display, TEXT("Progress Dialogue"));

	DialogueWidgetPtr->GetScrollBox()->ClearChildren();
	DialogueWidgetPtr->SetDialogueText((DialogueTable.bIsPlayerDialogue ? FText::FromString(TEXT("데인 코너")) : ActorName), DialogueTable.Dialogue);

	if (DialogueTable.bIsEnd)
	{
		DialogueWidgetPtr->CloseNextDialogueButton();

		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
			{
				DialogueWidgetPtr->RemoveFromParent();
				OnFinishedDialogue.ExecuteIfBound();
				InputModeGameOnly();
				if (IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(Character))
				{
					if (CII && CII->GetClassName() == TEXT("Speedwagon"))
						Character->GetCharacterMovement()->MaxWalkSpeed = 0.f;
					else
						Character->GetCharacterMovement()->MaxWalkSpeed = 300.f;
				}
				GetAIController()->MoveToNextPoint();
			}, 3.f, false);

		FZeroDialogueDataTable* FoundRow = DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(PrevIndex, FString());
		DialogueTable = *FoundRow;

		bIsTalking = false;
		bIsInterview = false;
		GetPlayerController()->SelectedInterviewName = "";
	}

	DialogueDataInit();
}

void UZeroDialogueComponent::NextDayDialogue(uint8 InDay)
{
	while (true)
	{
		// 대화 DataTable의 Index를 하나씩 증가시키며 현재 날짜와 동일한지 검사
		int32 RowIndexNum = FCString::Atoi(*RowIndex.ToString());
		RowIndexNum++;
		RowIndex = FName(*FString::Printf(TEXT("%d"), RowIndexNum));

		FString ContextString(TEXT("Dialogue Context"));
		FZeroDialogueDataTable* FoundRow = DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(RowIndex, ContextString);
		if (FoundRow == nullptr) return;
		DialogueTable = *FoundRow;

		// 대화 DataTable의 현재 Index의 데이터가 현재 날짜와 동일하게 나온다면
		// 해당 날의 DataTable로 변경 후 return
		if (DialogueTable.Day == InDay)
		{
			RowIndex = DialogueTable.RowIndex;
			bIsEnd = DialogueTable.bIsEnd;
			PrevIndex = DialogueTable.PrevIndex;
			return;
		}
	}
}

void UZeroDialogueComponent::StartInterviewDialogue(uint8 InReliabilityLevel)
{
	for (int i = -2; i <= 0; ++i)
	{
		FName TempIndex = FName(*FString::Printf(TEXT("%d"), i));
		if (DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(TempIndex, FString())->ReliabilityLevel == InReliabilityLevel)
		{
			if (DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(TempIndex, FString())->DataTable.IsPending())
			{
				DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(TempIndex, FString())->DataTable.LoadSynchronous();
			}

			DialogueTable = *DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(TempIndex, FString())->DataTable->FindRow<FZeroDialogueDataTable>(TEXT("1"), FString());
			return;
		}
	}
}

AZeroAIControllerNPC* UZeroDialogueComponent::GetAIController() const
{
	return Cast<AZeroAIControllerNPC>(Character->GetController());
}

void UZeroDialogueComponent::DialogueOptionSpawn(const FZeroDialogueOptionDataTable& InDialogueOptionTable)
{
	DialogueWidgetPtr->CloseNextDialogueButton();
	DialogueWidgetPtr->SetDialogueText((DialogueTable.bIsPlayerDialogue ? FText::FromString(TEXT("데인 코너")) : ActorName), DialogueTable.Dialogue);
	DialogueOptionWidgetPtr = CreateWidget<UZeroDialogueOptionWidget>(GetWorld(), DialogueOptionWidgetClass);
	DialogueOptionWidgetPtr->SetDialogueComp(this);
	DialogueOptionWidgetPtr->SetReliability(InDialogueOptionTable.Reliability);
	DialogueOptionDataInit(InDialogueOptionTable.DataTable, InDialogueOptionTable.RowIndex);
	
	if (InDialogueOptionTable.bIsInterview == false)
	{
		DialogueWidgetPtr->GetScrollBox()->AddChild(DialogueOptionWidgetPtr);
		DialogueOptionWidgetPtr->SetDialogueOptionText(InDialogueOptionTable.OptionDialogue);
	}
	else if (InDialogueOptionTable.bIsInterview == true && bIsInterview == true)
	{
		DialogueWidgetPtr->GetScrollBox()->AddChild(DialogueOptionWidgetPtr);
		DialogueOptionWidgetPtr->SetDialogueOptionText(InDialogueOptionTable.OptionDialogue);
	}
}

void UZeroDialogueComponent::DialogueDataInit()
{
	// 다음에 갈 대사 선택 하는거
	RowIndex = DialogueTable.RowIndex;
	bIsEnd = DialogueTable.bIsEnd;
	PrevIndex = DialogueTable.PrevIndex;

	// 1. DataTable이 유효한지 먼저 확인
	if (!DialogueTable.DataTable.IsValid())
	{
		DialogueTable.DataTable = DialogueTable.DataTable.LoadSynchronous();
	}

	// 2. DataTable이 여전히 유효하지 않다면 종료
	if (!DialogueTable.DataTable.IsValid())
	{
		return;
	}

	// 3. 데이터 할당
	FString ContextString(TEXT("Dialogue Context"));
	DialogueTable = *DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(RowIndex, ContextString);
	ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueTable.Dialogue.ToString());
}

void UZeroDialogueComponent::DialogueOptionDataInit(TSoftObjectPtr<UDataTable> InDataTable, FName InRowIndex)
{
	// 1. DataTable이 유효한지 먼저 확인
	if (!InDataTable.IsValid())
	{
		InDataTable = InDataTable.LoadSynchronous();
	}

	// 2. DataTable이 여전히 유효하지 않다면 종료
	if (!InDataTable.IsValid())
	{
		return;
	}

	// 3. 데이터 할당
	FString ContextString(TEXT("Dialogue Context"));
	DialogueOptionWidgetPtr->DialogueTableInOption = *InDataTable->FindRow<FZeroDialogueDataTable>(InRowIndex, ContextString);
	ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueOptionWidgetPtr->DialogueTableInOption.Dialogue.ToString());
}



