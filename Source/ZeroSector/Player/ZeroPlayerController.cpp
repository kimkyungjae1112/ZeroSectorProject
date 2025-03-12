// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZeroPlayerController.h"

AZeroPlayerController::AZeroPlayerController()
{
}

void AZeroPlayerController::InputModeGameOnly()
{
	bShowMouseCursor = false;
	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
}

void AZeroPlayerController::InputModeGameAndUI()
{
	bShowMouseCursor = true;
	FInputModeUIOnly UIOnly;
	SetInputMode(UIOnly);
}

void AZeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InputModeGameOnly();
}
