// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZeroPlayerController.h"

AZeroPlayerController::AZeroPlayerController()
{
}

void AZeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnly;
	SetInputMode(GameOnly);
}
