// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Gimmick/ZeroGimmick.h"
#include "ZeroOperationBoard.generated.h"

UCLASS()
class ZEROSECTOR_API AZeroOperationBoard : public AZeroGimmick
{
	GENERATED_BODY()
	
public:	
	AZeroOperationBoard();

protected:
	virtual void BeginPlay() override;


};
