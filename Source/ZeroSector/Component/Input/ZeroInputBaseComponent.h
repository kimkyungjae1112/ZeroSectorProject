// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZeroInputBaseComponent.generated.h"

struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROSECTOR_API UZeroInputBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZeroInputBaseComponent();

protected:
	virtual void BeginPlay() override;


public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run();
	void Walk();
	
protected:
	TObjectPtr<ACharacter> Player;
		
};
