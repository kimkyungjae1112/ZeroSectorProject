// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroUIComponent.h"
#include "Component/Input/ZeroInputAfternoonComponent.h"
#include "ZeroHeader/ZeroUIHeader.h"
#include "Interface/ZeroUIComponentInterface.h"
#include "Player/ZeroPlayerController.h"
#include "Data/ZeroSingleton.h"
#include "Data/ZeroProvisoDataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Gimmick/ZeroProvisoActor.h"
#include "UI/ZeroPauseMenuWidget.h"
#include "UI/ZeroExcludeResearcherWidget.h"
#include "UI/ZeroSelectResearcherWidget.h"
#include "UI/ZeroAfternoonHUDWidget.h"
#include "UI/ZeroEnforceBoardWidget.h"
#include "Game/ZeroGameModeBase.h"
#include "Component/ZeroPlayerStatComponent.h"
#include "ZeroSector.h"
#include "GameFramework/Character.h"


UZeroUIComponent::UZeroUIComponent()
{
	static ConstructorHelpers::FClassFinder<UZeroPauseMenuWidget> WidgetClass(TEXT("/Game/Blueprints/UI/WBP_PauseMenu"));
	if (WidgetClass.Succeeded())
	{
		PauseMenuWidgetClass = WidgetClass.Class;
	}
}


void UZeroUIComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ACharacter>(GetOwner());
	StatComp = Player->GetComponentByClass<UZeroPlayerStatComponent>(); // 바인딩이 된 포인터 가져오기

}

void UZeroUIComponent::FadeInAndOutDisplay()
{
	FadeInAndOutWidgetPtr = CreateWidget<UZeroFadeInAndOutWidget>(GetWorld(), FadeInAndOutWidgetClass);
	FadeInAndOutWidgetPtr->AddToViewport();
	FadeInAndOutWidgetPtr->FadeInPlay();
	FadeInAndOutWidgetPtr = nullptr;
}

void UZeroUIComponent::ToggleNoteDisplay()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());

	if (bIsNoteToggle)
	{
		if (NoteWidgetPtr && NoteWidgetPtr->IsInViewport())
		{
			NoteWidgetPtr->RemoveFromParent();
		}
		bIsNoteToggle = false;
		Interface->GetOwnerController()->RemoveBlurEffect();
		Interface->GetOwnerController()->InputModeGameOnly();
	}
	else
	{
		if (!NoteWidgetPtr)
		{
			NoteWidgetPtr = CreateWidget<UZeroNoteWidget>(GetWorld(), NoteWidgetClass);
			if (!NoteWidgetPtr)
			{
				return;
			}
		}

		if (!NoteWidgetPtr->IsInViewport())
		{
			NoteWidgetPtr->AddToViewport();

			NoteWidgetPtr->ClearResearcherInfo();
			NoteWidgetPtr->SetCurrentNoteResearcher(nullptr); 

			NoteWidgetPtr->ShowWidget();
			PendingProvisoList.Empty();
		}

		bIsNoteToggle = true;
		Interface->GetOwnerController()->ApplyBlurEffect();
		Interface->GetOwnerController()->InputModeGameAndUI();
	}
}

void UZeroUIComponent::InteractUIDisplay()
{
	if (!ProvisoWidgetPtr)
	{
		if (ProvisoWidgetClass)
		{
			ProvisoWidgetPtr = CreateWidget<UZeroProvisoWidget>(GetWorld(), ProvisoWidgetClass);
		}
	}
	if (ProvisoWidgetPtr)
	{
		ProvisoWidgetPtr->ShowWidget();
	}
}

void UZeroUIComponent::InteractUIClose()
{
	if (ProvisoWidgetPtr && ProvisoWidgetPtr->IsInViewport())
	{
		ProvisoWidgetPtr->RemoveFromParent();
	}
}

void UZeroUIComponent::OperationNextButtonClick()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeGameOnly();
		Interface->ChangeInputMode();
	}

	OnClickOperationNextButton.ExecuteIfBound(OperationWidgetPtr->GetWeaponType());
	OperationWidgetPtr->RemoveFromParent();
}

void UZeroUIComponent::OperationCancelButtonClick()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeGameOnly();
	}

	OperationWidgetPtr->RemoveFromParent();
}

void UZeroUIComponent::OperationInteract()
{
	if (AZeroGameModeBase::Day == 5 && bIsExclude)
	{
		ExcludeResearcherDisplay();
		return;
	}

	if (AZeroGameModeBase::Day == 7 && bIsSelect)
	{
		SelectResearcherDisplay();
		return;
	}

	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeUIOnly();
	}

	OperationWidgetPtr = CreateWidget<UZeroOperationWidget>(GetWorld(), OperationWidgetClass);
	if (OperationWidgetPtr)
	{
		OperationWidgetPtr->AddToViewport();
		OperationWidgetPtr->BackgroundImageChange(AZeroGameModeBase::Day);
		OperationWidgetPtr->OnCancelButton.BindUObject(this, &UZeroUIComponent::OperationCancelButtonClick);
		FOnClickNextButton OnClickNextButton;
		OnClickNextButton.BindLambda([&]()
			{
				OperationNextButtonClick();
			});
		OperationWidgetPtr->SetDelegateClickNextButton(OnClickNextButton);
	}

}

void UZeroUIComponent::ProvisoInteract()
{
	AZeroProvisoActor* ProvisoActor = Cast<AZeroProvisoActor>(CurrentGimmick);
	if (!ProvisoActor) return;

	CachedProvisoActor = ProvisoActor; 

	FZeroProvisoDataTable ProvisoData;
	if (!ProvisoActor->RowName.IsNone())
	{
		ProvisoData = UZeroSingleton::Get().GetProvisoData(ProvisoActor->RowName);
	}
	else
	{
		EZeroProvisoType Type = ProvisoActor->ProvisoType;
		ProvisoData = UZeroSingleton::Get().GetRandomProvisoByType(Type);
	}

	UZeroGetProvisoWidget* GetProvisoWidgetInstance = CreateWidget<UZeroGetProvisoWidget>(GetWorld(), GetProvisoWidgetClass);
	if (GetProvisoWidgetInstance)
	{
		IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
		if (Interface)
		{
			Interface->GetOwnerController()->InputModeUIOnly();
		}

		GetProvisoWidgetInstance->SetProvisoImage(ProvisoData.ProvisoImage.LoadSynchronous());
		GetProvisoWidgetInstance->SetProvisoInfo(ProvisoData.ProvisoName.ToString(), ProvisoData.Description);
		GetProvisoWidgetInstance->SetCurrentProvisoData(ProvisoData);

		GetProvisoWidgetInstance->OnProvisoConfirmed.AddLambda([this](const FZeroProvisoDataTable& Data)
			{
				if (CachedProvisoActor) CachedProvisoActor->Destroy();

				if (Data.ProvisoType != EZeroProvisoType::Fake && !Data.ProvisoName.IsNone())
				{
					UZeroSingleton::Get().AddCollectedProviso(Data);
				}

				if (StatComp)
				{
					StatComp->UseActivePoint(-10.f);
				}
			});

		GetProvisoWidgetInstance->OnProvisoRejected.AddLambda([this]()
			{
				if (CachedProvisoActor) CachedProvisoActor->Destroy();
			});

		GetProvisoWidgetInstance->AddToViewport();
	}
}




void UZeroUIComponent::EnforceBoardInteract()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeUIOnly();
	}

	EnforceBoardPtr = CreateWidget<UZeroEnforceBoardWidget>(GetWorld(), EnforceBoardClass);
	if (EnforceBoardPtr)
	{
		EnforceBoardPtr->AddToViewport();
		EnforceBoardPtr->SetPointText();
		EnforceBoardPtr->OnCancelButton.BindUObject(this, &UZeroUIComponent::EnforceBoardCancelButtonClick);
	}
}

void UZeroUIComponent::EnforceBoardCancelButtonClick()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeGameOnly();
	}

	if (EnforceBoardPtr && EnforceBoardPtr->IsInViewport())
	{
		EnforceBoardPtr->RemoveFromParent();
	}
}


void UZeroUIComponent::PauseMenuDisplay()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;

	UZeroPauseMenuWidget* PauseWidget = CreateWidget<UZeroPauseMenuWidget>(GetWorld(), PauseMenuWidgetClass);
	if (PauseWidget)
	{
		PauseWidget->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		Interface->GetOwnerController()->InputModeUIOnly();
	}
}

void UZeroUIComponent::ExcludeResearcherDisplay()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeUIOnly();
		Interface->GetOwnerController()->ApplyBlurEffect();
	}

	UZeroExcludeResearcherWidget* ExcludeResearcherWidget = CreateWidget<UZeroExcludeResearcherWidget>(GetWorld(), ExcludeResearcherWidgetClass);
	if (ExcludeResearcherWidget)
	{
		ExcludeResearcherWidget->AddToViewport();
		ExcludeResearcherWidget->OnCloseExclude.BindUObject(this, &UZeroUIComponent::ExcludeAfterOperation);
	}
}

void UZeroUIComponent::SelectResearcherDisplay()
{
	IZeroUIComponentInterface* Interface = Cast<IZeroUIComponentInterface>(GetOwner());
	if (Interface)
	{
		Interface->GetOwnerController()->InputModeUIOnly();
		Interface->GetOwnerController()->ApplyBlurEffect();
	}

	UZeroSelectResearcherWidget* SelectResearcherWidget = CreateWidget<UZeroSelectResearcherWidget>(GetWorld(), SelectResearcherWidgetClass);
	if (SelectResearcherWidget)
	{
		SelectResearcherWidget->AddToViewport();
		SelectResearcherWidget->OnCloseSelect.BindUObject(this, &UZeroUIComponent::SelectAfterOperation);
	}
}

void UZeroUIComponent::ExcludeAfterOperation()
{
	bIsExclude = false;
	OperationInteract();
}

void UZeroUIComponent::SelectAfterOperation()
{
	bIsSelect = false;
	OperationInteract();
}



