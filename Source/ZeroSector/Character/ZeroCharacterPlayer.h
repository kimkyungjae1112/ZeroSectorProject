// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "Weapon/ZeroWeaponType.h"
#include "Environment/ZeroDayType.h"
#include "Interface/ZeroHUDInterface.h"
#include "Interface/ZeroAfternoonInputInterface.h"
#include "Interface/ZeroNightInputInterface.h"
#include "Interface/ZeroUIComponentInterface.h"
#include "ZeroCharacterPlayer.generated.h"

struct FInputActionValue;
class UZeroInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UZeroHUDWidget;
class UZeroInputBaseComponent;
class UZeroUIComponent;
class APlayerController;
class AZeroPlayerController;

DECLARE_DELEGATE(FChangeInput)

USTRUCT()
struct FChangeInputWrapper
{
	GENERATED_BODY()

	FChangeInputWrapper() {}
	FChangeInputWrapper(const FChangeInput& InChangeInput) : ChangeInput(InChangeInput) {}

	FChangeInput ChangeInput;
};

UCLASS()
class ZEROSECTOR_API AZeroCharacterPlayer 
	: public AZeroCharacterBase
	, public IZeroHUDInterface
	, public IZeroAfternoonInputInterface
	, public IZeroNightInputInterface
	, public IZeroUIComponentInterface
{
	GENERATED_BODY()
	
public:
	AZeroCharacterPlayer();

	virtual void Tick(float DeltaSeconds) override;

	/* APawn Overriding */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

	/* IZeroHUDInterface Implement */
	virtual void SetHUDWidget(UZeroHUDWidget* InHUDWidget) override;

	/* IZeroAfternoonInputInterface Implemnt */
	virtual void DisplayInteractUI() override;
	virtual void CloseInteractUI() override;

	/* IZeroNightInputInterface Implemnt */
#if WITH_EDITOR
	virtual void NightToAfternoon() override;
#endif

	/* IZeroUIComponentInterface Implement */
	virtual class AZeroPlayerController* GetOwnerController() override;
	virtual void ChangeInputMode() override;

	/* APawn override */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	

/* 포인터를 얻어오는 유틸리티 함수 */
private:
	APlayerController* GetPlayerController() const;
	AZeroPlayerController* GetZeroPlayerController() const;


/* Input 바인딩 함수 */
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Run();
	void Walk();
	void Fire();
	void ChangeWeapon();
	void Reloading();

	void DialogueInteract();
	void ProvisoInteract();
	void OperationBoardInteract();

	void ToggleNoteDisplay();


/* Input 데이터 및 변경 */
private:
	void SetInputByDaySequence(EDaySequence DaySequence);
	void SetInputAfternoonMode();
	void SetInputNightMode();

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TMap<EDaySequence, FChangeInputWrapper> ChangeInputMap;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UZeroInputConfig> InputConfig;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UZeroInputBaseComponent> InputComp;

/* Input Delegate 관리 */
private:
	void AfternoonInputDelegate();
	
/* 무기 */
private:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE EWeaponType GetWeaponType() const { return CurrentWeaponType; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponType CurrentWeaponType;


/* 카메라 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	
/* UI */
private:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroHUDWidget> HUDWidgetPtr;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroUIComponent> UIComp;


};
