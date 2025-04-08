// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroUIComponent.generated.h"

class UZeroFadeInAndOutWidget;
class UZeroOperationWidget;
class UZeroProvisoWidget;
class UZeroGetProvisoWidget;
class UZeroNoteWidget;
class UZeroPauseMenuWidget;
class UZeroExcludeResearcherWidget;
class UZeroSelectResearcherWidget;

DECLARE_DELEGATE_OneParam(FOnClickOperationNextButton, const EWeaponType&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROSECTOR_API UZeroUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UZeroUIComponent();

protected:
	virtual void BeginPlay() override;

public:
	FOnClickOperationNextButton OnClickOperationNextButton;

public:
	void FadeInAndOutDisplay();
	void ToggleNoteDisplay();
	void InteractUIDisplay();
	void InteractUIClose();

	void OperationNextButtonClick();
	void OperationInteract();
	void ProvisoInteract();
	void PauseMenuDisplay();
	void ExcludeResearcherDisplay();
	void SelectResearcherDisplay();

private:
	void ExcludeAfterOperation();
	void SelectAfterOperation();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UZeroOperationWidget> OperationWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroOperationWidget> OperationWidgetPtr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UZeroFadeInAndOutWidget> FadeInAndOutWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroFadeInAndOutWidget> FadeInAndOutWidgetPtr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZeroProvisoWidget> ProvisoWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroProvisoWidget> ProvisoWidgetPtr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZeroGetProvisoWidget> GetProvisoWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZeroNoteWidget> NoteWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroNoteWidget> NoteWidgetPtr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZeroPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZeroExcludeResearcherWidget> ExcludeResearcherWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UZeroSelectResearcherWidget> SelectResearcherWidgetClass;

	int32 ProvisoNum = 0;
	bool bIsNoteToggle = false;

	bool bIsExclude = true;
	bool bIsSelect = true;
};
