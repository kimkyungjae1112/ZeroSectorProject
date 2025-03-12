// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroDialogueComponent.h"
#include "Data/ZeroSingleton.h"
#include "Data/ZeroDialogueOptionDataTable.h"
#include "UI/ZeroDialogueWidget.h"
#include "UI/ZeroDialogueOptionWidget.h"
#include "Components/ScrollBox.h"
#include "Player/ZeroPlayerController.h"
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
}

void UZeroDialogueComponent::StartDialogue()
{
    RotationToPlayer();

	DialogueTable = UZeroSingleton::Get().GetDialogueTable(0);
    ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueTable.Dialogue.ToString());
	DialogueWidgetPtr = CreateWidget<UZeroDialogueWidget>(GetWorld(), DialogueWidgetClass);
	DialogueWidgetPtr->AddToViewport();
	DialogueWidgetPtr->SetDialogueText(ActorName, DialogueTable.Dialogue);

    if (DialogueTable.bIsOpenOption)
    {
        InputModeGameAndUI();
        for (const auto& DialogueOptionTable : DialogueTable.OptionDialogues)
        {
            DialogueOptionSpawn(DialogueOptionTable);
        }
        return;
    }
}

void UZeroDialogueComponent::SetupFinishedDialogueDelegate(const FOnFinishedDialogue& InOnFinishedDialogue)
{
    OnFinishedDialogue = InOnFinishedDialogue;
}

void UZeroDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UZeroDialogueComponent::OnClickedOption(FZeroDialogueDataTable InDialogueTable)
{
    DialogueWidgetPtr->GetScrollBox()->ClearChildren();

    DialogueTable = InDialogueTable;
    ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueTable.Dialogue.ToString());
    
    DialogueWidgetPtr->UpdateDialogue(DialogueTable.Dialogue);
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
        
        FTimerHandle Timer;
        GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
            {
                DialogueWidgetPtr->RemoveFromParent();
                OnFinishedDialogue.ExecuteIfBound();
                InputModeGameOnly();
            }, 3.f, false);
        DialogueTable = UZeroSingleton::Get().GetDialogueTable(0);
    }
}

void UZeroDialogueComponent::RotationToPlayer()
{
    AActor* Owner = GetOwner();
    APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (Owner && Player)
    {
        FVector Dir = Player->GetActorLocation() - Owner->GetActorLocation();
        Owner->SetActorRotation(Dir.Rotation());
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

void UZeroDialogueComponent::DialogueOptionSpawn(const FZeroDialogueOptionDataTable& InDialogueOptionTable)
{
    DialogueOptionWidgetPtr = CreateWidget<UZeroDialogueOptionWidget>(GetWorld(), DialogueOptionWidgetClass);
    DialogueOptionWidgetPtr->SetDialogueComp(this);
    DialogueOptionDataInit(InDialogueOptionTable.DataTable, InDialogueOptionTable.RowIndex);
    DialogueWidgetPtr->GetScrollBox()->AddChild(DialogueOptionWidgetPtr);
    DialogueOptionWidgetPtr->SetDialogueOptionText(InDialogueOptionTable.OptionDialogue);
}

void UZeroDialogueComponent::DialogueDataInit()
{
    RowIndex = DialogueTable.RowIndex;
    bIsEnd = DialogueTable.bIsEnd;

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

    // 3. FindRow 호출
    FString ContextString(TEXT("Dialogue Context"));
    FZeroDialogueDataTable* FoundRow = DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(RowIndex, ContextString);

    // 4. 데이터 할당
    DialogueTable = *FoundRow;
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

    // 3. FindRow 호출
    FString ContextString(TEXT("Dialogue Context"));
    FZeroDialogueDataTable* FoundRow = InDataTable->FindRow<FZeroDialogueDataTable>(InRowIndex, ContextString);

    // 4. 데이터 할당
    DialogueOptionWidgetPtr->DialogueTableInOption = *FoundRow;
    ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueOptionWidgetPtr->DialogueTableInOption.Dialogue.ToString());
}





