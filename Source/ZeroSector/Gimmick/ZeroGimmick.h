// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ZeroOutlineInterface.h"
#include "ZeroGimmick.generated.h"

class UBoxComponent;
class UMaterialInstance;

UCLASS(abstract)
class ZEROSECTOR_API AZeroGimmick 
	: public AActor
	, public IZeroOutlineInterface
{
	GENERATED_BODY()
	
public:	
	AZeroGimmick();

	virtual void SetOverlayMaterial() override;
	virtual void SetUnOverlayMaterial() override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<UStaticMeshComponent> MeshComp;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Material")
	TObjectPtr<UMaterialInstance> OutlineMaterial;
};
