// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/ZeroWeaponType.h"
#include "ZeroInputBaseComponent.generated.h"

struct FInputActionValue;

DECLARE_DELEGATE(FOnOperationInteract)
DECLARE_DELEGATE(FOnProvisoInteract)
DECLARE_DELEGATE(FOnNoteDisplay)
DECLARE_DELEGATE(FOnPauseMenu)
DECLARE_DELEGATE(FOnExcludeResearcher)
DECLARE_DELEGATE(FOnSelectResearcher)

UCLASS( abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROSECTOR_API UZeroInputBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/* Common */
	virtual void Move(const FInputActionValue& Value) PURE_VIRTUAL(UZeroInputBaseComponent::Move, );
	virtual void Look(const FInputActionValue& Value) PURE_VIRTUAL(UZeroInputBaseComponent::Look, );
	virtual void Run() PURE_VIRTUAL(UZeroInputBaseComponent::Run, );
	virtual void Walk() PURE_VIRTUAL(UZeroInputBaseComponent::Walk, );


	/* Afternoon */
	virtual void InteractBeam() PURE_VIRTUAL(UZeroInputBaseComponent::InteractBeam, );
	virtual void InteractProcess(const FHitResult& InHitResult, bool bIsHit) PURE_VIRTUAL(UZeroInputBaseComponent::InteractProcess, );
	virtual void InteractBeamReachedProviso(AActor* InHitActor) PURE_VIRTUAL(UZeroInputBaseComponent::InteractBeamReachedProviso, );
	virtual void DialogueInteract() PURE_VIRTUAL(UZeroInputBaseComponent::DialogueInteract, );
	virtual void ProvisoInteract() PURE_VIRTUAL(UZeroInputBaseComponent::ProvisoInteract, );
	virtual void OperationBoardInteract() PURE_VIRTUAL(UZeroInputBaseComponent::OperationBoardInteract, );
	virtual void ToggleNote() PURE_VIRTUAL(UZeroInputBaseComponent::ToggleNote, );
	virtual void ExcludeResearcher() PURE_VIRTUAL(UZeroInputBaseComponent::ExcludeResearcher, );
	virtual void SelectResearcher() PURE_VIRTUAL(UZeroInputBaseComponent::SelectResearcher, );

	FOnOperationInteract OnOperationInteract;
	FOnProvisoInteract OnProvisoInteract;
	FOnNoteDisplay OnNoteDisplay;
	FOnExcludeResearcher OnExcludeResearcher;
	FOnSelectResearcher OnSelectResearcher;

	/* Night */
	virtual void Fire() PURE_VIRTUAL(UZeroInputBaseComponent::Fire, );
	virtual void ChangeWeapon() PURE_VIRTUAL(UZeroInputBaseComponent::ChangeWeapon, );
	virtual void Reloading() PURE_VIRTUAL(UZeroInputBaseComponent::Reloading, );
	virtual void SetupWeapon(const EWeaponType& WeaponType) PURE_VIRTUAL(UZeroInputBaseComponent::SetupWeapon, );
	virtual void SetUnequipWeapon() PURE_VIRTUAL(UZeroInputBaseComponent::SetUnequipWeapon, );
	virtual EWeaponType GetWeaponType() PURE_VIRTUAL(UZeroInputBaseComponent::GetWeaponType, return EWeaponType::EPistol;);

	/* Menu */
	virtual void PauseMenu() PURE_VIRTUAL(UZeroInputBaseComponent::PauseMenu, );
	
	FOnPauseMenu OnPauseMenu;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> Player;

};
