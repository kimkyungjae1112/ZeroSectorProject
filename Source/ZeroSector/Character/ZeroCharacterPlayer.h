// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "ZeroCharacterPlayer.generated.h"

struct FInputActionValue;
class UZeroInputConfig;
class APlayerController;
class UCameraComponent;

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

/* 카메라 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

/* 입력 데이터 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Input Data")
	TObjectPtr<UZeroInputConfig> InputConfig;

};
