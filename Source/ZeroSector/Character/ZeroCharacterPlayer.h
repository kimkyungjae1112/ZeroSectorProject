// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "ZeroCharacterPlayer.generated.h"

struct FInputActionValue;
class IZeroDialogueInterface;
class UZeroInputConfig;
class UCameraComponent;
class UZeroPlayerCameraData;
class UZeroFadeInAndOutWidget;
class UZeroOperationWidget;
class UZeroProvisoWidget;
class UZeroGetProvisoWidget;
class AZeroGimmick;
class AZeroWeaponBase;
class APlayerController;


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
class ZEROSECTOR_API AZeroCharacterPlayer : public AZeroCharacterBase
{
	GENERATED_BODY()
	
public:
	AZeroCharacterPlayer();

	virtual void Tick(float DeltaSeconds) override;

	/* APawn Overriding */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/* IGenericTeamAgentInterface Implement */
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	virtual void BeginPlay() override;


/* 포인터를 얻어오는 유틸리티 함수 */
private:
	APlayerController* GetPlayerController() const;


/* 어빌리티 함수들 */
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Fire();
	void Aiming();
	void UnAiming();

	/* DialogueInteract() 를 이루는 함수 */
	void SetDefaultMovement();
	void SetDialogueMovement();


	/* 시선 앞의 물체 탐색 함수 */
	void InteractBeam();

	/* 상호작용 함수들 */
	void DialogueInteract();
	void ProvisoInteract();
	void OperationBoardInteract();

/* 카메라 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UZeroPlayerCameraData> CameraData;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float DetectDistance;

/* 대화 섹션 */
private:
	IZeroDialogueInterface* DialogueInterface;

/* Input */
private:
	void SetInputByDaySequence(EDaySequence DaySequence);
	void SetInputAfternoonMode();
	void SetInputNightMode();

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TMap<EDaySequence, FChangeInputWrapper> ChangeInputMap;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UZeroInputConfig> InputConfig;

/* 기믹 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Gimmick")
	TObjectPtr<AZeroGimmick> InteractedGimmick;

	UZeroProvisoWidget* ProvisoWidgetInstance; 
	UZeroGetProvisoWidget* GetProvisoWidgetInstance;

/* 무기 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AZeroWeaponBase> Weapon;

	bool bIsAiming = false;

/* UI */
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
	TSubclassOf<UUserWidget> ProvisoWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> GetProvisoWidgetClass;


/* 작전창 UI를 띄울 테스트 함수 */
	void OperationUITest();
	void ClickNextButton();
	
	
};
