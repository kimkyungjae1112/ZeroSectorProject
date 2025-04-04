// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroDialogueComponent.h"
#include "Data/ZeroSingleton.h"
#include "Data/ZeroDialogueOptionDataTable.h"
#include "UI/ZeroDialogueWidget.h"
#include "UI/ZeroDialogueOptionWidget.h"
#include "Components/ScrollBox.h"
#include "Player/ZeroPlayerController.h"
#include "Interface/ZeroClassIdentifierInterface.h"
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

    S_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_S.DialogueDataTable_S'"));
	V_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_V.DialogueDataTable_V'"));
	C_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_C.DialogueDataTable_C'"));
	N1_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_N1.DialogueDataTable_N1'"));
	N2_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_N2.DialogueDataTable_N2'"));
	N3_DialogueTable = FSoftObjectPath(TEXT("/Script/Engine.DataTable'/Game/Data/Dialogue/DialogueDataTable_N3.DialogueDataTable_N3'"));

    DialogueTableMap.Add(TEXT("Speedwagon"), S_DialogueTable);
    DialogueTableMap.Add(TEXT("Vaccine"), V_DialogueTable);
    DialogueTableMap.Add(TEXT("Criminal"), C_DialogueTable);
    DialogueTableMap.Add(TEXT("Normal1"), N1_DialogueTable);
    DialogueTableMap.Add(TEXT("Normal2"), N2_DialogueTable);
    DialogueTableMap.Add(TEXT("Normal3"), N3_DialogueTable);
}

void UZeroDialogueComponent::StartDialogue()
{
    if (bIsTalking) InProgressDialogue();

    RotationToPlayer();

    ZE_LOG(LogZeroSector, Warning, TEXT("Dialogue Loaded: %s"), *DialogueTable.Dialogue.ToString());
    bIsTalking = true;
    DialogueWidgetPtr = CreateWidget<UZeroDialogueWidget>(GetWorld(), DialogueWidgetClass);
	DialogueWidgetPtr->AddToViewport();
	DialogueWidgetPtr->SetDialogueText(ActorName, DialogueTable.Dialogue);

    if (DialogueTable.bIsOpenOption)
    {
        InputModeUIOnly();
        for (const auto& DialogueOptionTable : DialogueTable.OptionDialogues)
        {
            DialogueOptionSpawn(DialogueOptionTable);
        }
        return;
    }

    DialogueDataInit();
}

void UZeroDialogueComponent::SetupFinishedDialogueDelegate(const FOnFinishedDialogue& InOnFinishedDialogue)
{
    OnFinishedDialogue = InOnFinishedDialogue;
}

void UZeroDialogueComponent::BeginPlay()
{
	Super::BeginPlay();

    if (IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(GetOwner()))
    {
        if (DialogueTableMap[CII->GetClassName()].IsPending())
        {
            FString DialogueContext(TEXT("Dialogue Context"));
            DialogueTable = *DialogueTableMap[CII->GetClassName()].LoadSynchronous()->FindRow<FZeroDialogueDataTable>(TEXT("1"), DialogueContext);
            PrevIndex = DialogueTable.PrevIndex;
        }
    }
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

        FString ContextString(TEXT("Dialogue Context"));
        FZeroDialogueDataTable* FoundRow = DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(DialogueTable.PrevIndex, ContextString);
        DialogueTable = *FoundRow;
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
    DialogueWidgetPtr->GetScrollBox()->ClearChildren();
    DialogueWidgetPtr->UpdateDialogue(DialogueTable.Dialogue);
       
    if (DialogueTable.bIsEnd)
    {
        FTimerHandle Timer;
        GetWorld()->GetTimerManager().SetTimer(Timer, [&]()
            {
                DialogueWidgetPtr->RemoveFromParent();
                OnFinishedDialogue.ExecuteIfBound();
                InputModeGameOnly();
            }, 3.f, false);

        FString ContextString(TEXT("Dialogue Context"));
        FZeroDialogueDataTable* FoundRow = DialogueTable.DataTable->FindRow<FZeroDialogueDataTable>(DialogueTable.PrevIndex, ContextString);
        DialogueTable = *FoundRow;
    }

    DialogueDataInit();
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





