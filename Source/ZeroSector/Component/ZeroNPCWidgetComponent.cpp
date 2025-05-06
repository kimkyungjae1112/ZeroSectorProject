// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroNPCWidgetComponent.h"
#include "UI/ZeroNPCNameWidget.h"

void UZeroNPCWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UZeroNPCNameWidget* UserWidget = Cast<UZeroNPCNameWidget>(GetWidget());
	if (UserWidget)
	{
		UserWidget->SetOwningActor(GetOwner());
	}
}
