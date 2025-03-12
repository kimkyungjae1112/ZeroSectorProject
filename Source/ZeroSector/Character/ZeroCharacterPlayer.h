// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ZeroCharacterBase.h"
#include "ZeroCharacterPlayer.generated.h"

struct FInputActionValue;
class IZeroDialogueInterface;
class APlayerController;
class UZeroInputConfig;
class UCameraComponent;
class USphereComponent;
class UZeroPlayerCameraData;

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

	/* DialogueInteract() 를 이루는 함수 */
	void SetDefaultMovement();
	void SetDialogueMovement();


/* 카메라 */
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UZeroPlayerCameraData> CameraData;

/* 대화 섹션 */
private:
	UFUNCTION()
	void BeginOverlapForDialogue(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Collision")
	TObjectPtr<USphereComponent> DialogueSphereComp;

	IZeroDialogueInterface* DialogueInterface;

/* Input */
private:
	UPROPERTY(VisibleAnywhere, Category = "Input Data")
	TObjectPtr<UZeroInputConfig> InputConfig;

};
