// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroGetProvisoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Interface/ZeroUIComponentInterface.h"
#include "UI/ZeroMessageWidget.h"

void UZeroGetProvisoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (WriteButton)
	{
		WriteButton->OnClicked.AddDynamic(this, &UZeroGetProvisoWidget::OnWriteClicked);
	}

	if (ThrowButton)
	{
		ThrowButton->OnClicked.AddDynamic(this, &UZeroGetProvisoWidget::OnThrowClicked);
	}
}

void UZeroGetProvisoWidget::OnWriteClicked()
{
    if (CurrentProvisoData.ProvisoType == EZeroProvisoType::Fake)
    {
        UE_LOG(LogTemp, Warning, TEXT("FF"));

        OnProvisoRejected.Broadcast(); 

        RemoveFromParent();

        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        if (PC)
        {
            PC->SetInputMode(FInputModeGameOnly());
            PC->bShowMouseCursor = false;
        }

        // 쓸모없는 정보 메시지 표시
        if (MessageWidgetClass)
        {
            UZeroMessageWidget* MessageWidget = CreateWidget<UZeroMessageWidget>(GetWorld(), MessageWidgetClass);
            if (MessageWidget)
            {
                MessageWidget->AddToViewport();

                FTimerHandle TempHandle;
                FTimerDelegate Delegate;
                Delegate.BindLambda([MessageWidget]()
                    {
                        MessageWidget->RemoveFromParent();
                    });

                GetWorld()->GetTimerManager().SetTimer(TempHandle, Delegate, 2.0f, false);
            }
        }

        return;
    }

    OnProvisoConfirmed.Broadcast(CurrentProvisoData);
    RemoveFromParent();

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PC->SetInputMode(FInputModeGameOnly());
        PC->bShowMouseCursor = false;
    }
}


void UZeroGetProvisoWidget::OnThrowClicked()
{
	OnProvisoRejected.Broadcast();
	RemoveFromParent();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetInputMode(FInputModeGameOnly());
		PC->bShowMouseCursor = false;
	}
}

void UZeroGetProvisoWidget::SetProvisoInfo(const FString& ProvisoName, const FString& ProvisoDescription)
{
	if (ProvisoNameText)
	{
		ProvisoNameText->SetText(FText::FromString(ProvisoName));
	}

	if (ProvisoDescriptionText)
	{
		ProvisoDescriptionText->SetText(FText::FromString(ProvisoDescription));
	}
}

void UZeroGetProvisoWidget::SetProvisoImage(UTexture2D* InImage)
{
	if (ProvisoImage && InImage)
	{
		ProvisoImage->SetBrushFromTexture(InImage);
	}
}

void UZeroGetProvisoWidget::SetCurrentProvisoData(const FZeroProvisoDataTable& InData)
{
	CurrentProvisoData = InData;
}

void UZeroGetProvisoWidget::ShowWidget()
{
    AddToViewport();
}



