// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "Weapon/ZeroWeaponType.h"
#include "Interface/ZeroHUDInterface.h"
#include "ZeroCharacterPlayer.generated.h"

struct FInputActionValue;
class IZeroDialogueInterface;
class UZeroInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UZeroPlayerCameraData;
class UZeroFadeInAndOutWidget;
class UZeroOperationWidget;
class UZeroProvisoWidget;
class UZeroGetProvisoWidget;
class UZeroNoteWidget;
class UZeroCrossHairWidget;
class UZeroHUDWidget;
class AZeroGimmick;
class AZeroWeaponBase;
class APlayerController;
class AZeroPlayerController;

/* 
	Component 변수 선언할 때 Comp로 줄이기
	CameraComponent -> CameraComp
*/

DECLARE_DELEGATE(FChangeInput)

UENUM()
enum class EDaySequence : uint8
{
	EAfternoon,
	ENight
};

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

protected:
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
	void Aiming();
	void UnAiming();
	void ChangeWeapon();
	void Reloading();

	void DialogueInteract();
	void ProvisoInteract();
	void OperationBoardInteract();

/* Input 데이터 및 변경 */
private:
	void SetInputByDaySequence(EDaySequence DaySequence);
	void SetInputAfternoonMode();
	void SetInputNightMode();

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TMap<EDaySequence, FChangeInputWrapper> ChangeInputMap;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UZeroInputConfig> InputConfig;

/* 상호작용 */
	void InteractBeam();
	void InteractProcess(const FHitResult& InHitResult, bool bIsHit);
	void InteractBeamReachedProviso(AActor* InHitActor);

	UPROPERTY(EditAnywhere, Category = "Interact")
	float DetectDistance;

/* 대화 섹션 */
private:
	void SetDefaultMovement();
	void SetDialogueMovement();

	IZeroDialogueInterface* DialogueInterface;


/* 기믹 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<AZeroGimmick> InteractedGimmick;

	
/* 무기 */
private:
	void SetNoWeapon();
	void SetRifle();
	void SetPistol();
	void SetShotgun();
	void SetupTransformWeapon(const FName& SocketName);
	void ChangeWeaponMesh();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	FORCEINLINE EWeaponType GetWeaponType() const { return CurrentWeaponType; }

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TMap<EWeaponType, AZeroWeaponBase*> Weapons;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<AZeroWeaponBase> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	EWeaponType CurrentWeaponType;

	EWeaponType ChoicedWeapon;
	bool bIsAiming = false;

/* 카메라 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UZeroPlayerCameraData> CameraData;

	
/* UI */
private:
	void OperationWidgetDisplay();
	void OperationNextButtonClick();
	void FadeInAndOutDisplay();

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

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UZeroHUDWidget> HUDWidgetPtr;

/* Test Code */
	void NightToAfternoon();

	
/* 개인 수첩 */
	void ToggleNote();
	int8 bIsNoteToggle : 1;
	int32 ProvisoNum = 0;

};
