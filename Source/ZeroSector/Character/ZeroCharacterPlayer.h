// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "Game/ProvisoActor.h"
#include "ZeroCharacterPlayer.generated.h"

struct FInputActionValue;
class IZeroDialogueInterface;
class UZeroInputConfig;
class UCameraComponent;
class UZeroPlayerCameraData;
class UZeroFadeInAndOutWidget;
class UZeroOperationWidget;
class AZeroWeaponBase;
class AZeroOperationBoard;
class APlayerController;


/* 
	Component 변수 선언할 때 Comp로 줄이기
	CameraComponent -> CameraComp
*/

UCLASS()
class ZEROSECTOR_API AZeroCharacterPlayer : public AZeroCharacterBase
{
	GENERATED_BODY()
	
public:
	AZeroCharacterPlayer();

	virtual void Tick(float DeltaSeconds) override;

	/* APawn Overriding */
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;


/* 포인터를 얻어오는 유틸리티 함수 */
private:
	APlayerController* GetPlayerController() const;


/* 어빌리티 함수들 */
private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void DialogueInteract();
	void OperationBoardInteract();
	void Fire();
	void Aiming();

	/* DialogueInteract() 를 이루는 함수 */
	void SetDefaultMovement();
	void SetDialogueMovement();


	/* 시선 앞의 물체 탐색 함수 */
	void InteractBeam();
	void ProvisoInteract();
	// void ShowInteractionUI(bool bShow);

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
	UPROPERTY(VisibleAnywhere, Category = "Input Data")
	TObjectPtr<UZeroInputConfig> InputConfig;

/* 단서 데이터 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Proviso")
	TObjectPtr<AProvisoActor> DetectedProviso;

/* 무기 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<AZeroWeaponBase> Weapon;

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

/* 작전판 */
private:
	UPROPERTY(VisibleAnywhere, Category = "OperationBoard")
	TObjectPtr<AZeroOperationBoard> OperationBoard;

/* 작전창 UI를 띄울 테스트 함수 */
	void OperationUITest();
	void ClickNextButton();

};
