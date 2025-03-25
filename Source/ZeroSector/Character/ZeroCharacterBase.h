// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "Interface/ZeroClassIdentifierInterface.h"
#include "ZeroCharacterBase.generated.h"

UCLASS(abstract)
class ZEROSECTOR_API AZeroCharacterBase 
	: public ACharacter
	, public IGenericTeamAgentInterface
	, public IZeroClassIdentifierInterface
{
	GENERATED_BODY()

public:
	AZeroCharacterBase();

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

	/* IZeroClassIdentifierInterface Implement */
	virtual FName GetClassName() const override { return ClassName; }

protected:
	virtual void BeginPlay() override;

	FGenericTeamId TeamId;

	/* 스텟을 받기 위한 키 */
	UPROPERTY(VisibleAnywhere, Category = "Class")
	FName ClassName;
};
