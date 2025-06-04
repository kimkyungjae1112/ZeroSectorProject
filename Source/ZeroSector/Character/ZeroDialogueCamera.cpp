// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroDialogueCamera.h"
#include "Camera/CameraComponent.h"

AZeroDialogueCamera::AZeroDialogueCamera()
{
	DialogueCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("DialogueCamera"));
	RootComponent = DialogueCamera;
}


