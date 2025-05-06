// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ZeroNPCNameWidget.h"
#include "Components/TextBlock.h"
#include "Interface/ZeroClassIdentifierInterface.h"

UZeroNPCNameWidget::UZeroNPCNameWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UZeroNPCNameWidget::SetNameText(const FName& InName)
{
	NameText->SetText(FText::FromName(InName));
}

void UZeroNPCNameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("NameTextBlock")));

	ensure(NameText);

	IZeroClassIdentifierInterface* Interface = Cast<IZeroClassIdentifierInterface>(OwningActor);
	if (Interface)
	{
		SetNameText(Interface->GetClassName());
	}
}
