// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Input/ZeroInputNightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Interface/ZeroNightInputInterface.h"
#include "ZeroHeader/ZeroWeaponHeader.h"
#include "ZeroSector.h"

//#define TEMP

//임시
#include "Kismet/GameplayStatics.h"

UZeroInputNightComponent::UZeroInputNightComponent()
{
	CurrentWeaponType = EWeaponType::ENone;
}

void UZeroInputNightComponent::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	FRotator Rotation = Player->Controller->GetControlRotation();
	FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Player->AddMovementInput(ForwardVector, InputValue.X);
	Player->AddMovementInput(RightVector, InputValue.Y);
}

void UZeroInputNightComponent::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	Player->AddControllerPitchInput(-InputValue.Y);
	Player->AddControllerYawInput(InputValue.X);
}

void UZeroInputNightComponent::Run()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = 500.f;
}

void UZeroInputNightComponent::Walk()
{
	Player->GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void UZeroInputNightComponent::Fire()
{
	CurrentWeapon->Fire();
}

void UZeroInputNightComponent::ChangeWeapon()
{
	if (ChoicedWeapon == EWeaponType::ERifle)
	{
		if (CurrentWeaponType == EWeaponType::EPistol)
		{
			SetRifle();
			CurrentWeapon->GunAmmoTextDisplay();
		}
		else
		{
			SetPistol();
			CurrentWeapon->GunAmmoTextDisplay();
		}
	}
	else if (ChoicedWeapon == EWeaponType::EShotgun)
	{
		if (CurrentWeaponType == EWeaponType::EPistol)
		{
			SetShotgun();
			CurrentWeapon->GunAmmoTextDisplay();
		}
		else
		{
			SetPistol();
			CurrentWeapon->GunAmmoTextDisplay();
		}
	}
}

void UZeroInputNightComponent::Reloading()
{
	CurrentWeapon->ReloadingCurrentAmmo();
}

void UZeroInputNightComponent::SetupWeapon(const EWeaponType& WeaponType)
{
	ZE_LOG(LogZeroSector, Warning, TEXT("SetupWeapon"));
	switch (WeaponType)
	{
	case EWeaponType::ERifle:
		Weapons.Add(EWeaponType::ERifle, GetWorld()->SpawnActor<AZeroWeaponRifle>(AZeroWeaponRifle::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		Weapons[EWeaponType::ERifle]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rRifle"));
		Weapons[EWeaponType::EPistol]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rPistol"));
		ChoicedWeapon = EWeaponType::ERifle;
		break;
	case EWeaponType::EShotgun:
		Weapons.Add(EWeaponType::EShotgun, GetWorld()->SpawnActor<AZeroWeaponShotgun>(AZeroWeaponShotgun::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		Weapons[EWeaponType::EShotgun]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rShotgun"));
		Weapons[EWeaponType::EPistol]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rPistol"));
		ChoicedWeapon = EWeaponType::EShotgun;
		break;
	default:
		ZE_LOG(LogZeroSector, Error, TEXT("무기 안들어옴"));
		break;
	}

	for (const auto& Weapon : Weapons)
	{
		Weapon.Value->SetOwner(Player);
		/*Weapon.Value->OnSetMaxAmmo.BindUObject(HUDWidgetPtr, &UZeroHUDWidget::UpdateMaxAmmo);
		Weapon.Value->OnChangedAmmo.BindUObject(HUDWidgetPtr, &UZeroHUDWidget::UpdateCurrentAmmo);*/
	}

	SetPistol();
}

EWeaponType UZeroInputNightComponent::GetWeaponType()
{
	return CurrentWeaponType;
}

void UZeroInputNightComponent::NightToAfternoon()
{
	SetNoWeapon();
}

void UZeroInputNightComponent::PauseMenu()
{
	UE_LOG(LogTemp, Warning, TEXT("pause"));
	OnPauseMenu.ExecuteIfBound();
}

void UZeroInputNightComponent::BeginPlay()
{
	Super::BeginPlay();

	check(Player);
}

void UZeroInputNightComponent::SetNoWeapon()
{
	CurrentWeaponType = EWeaponType::ENone;
	CurrentWeapon = nullptr;

	for (const auto& Weapon : Weapons)
	{
		Weapon.Value->Destroy();
	}
}

void UZeroInputNightComponent::SetRifle()
{
	CurrentWeaponType = EWeaponType::ERifle;
	CurrentWeapon = Weapons[EWeaponType::ERifle];

	SetupTransformWeapon(TEXT("hand_rRifle"));
	ChangeWeaponMesh();
}

void UZeroInputNightComponent::SetPistol()
{
	CurrentWeaponType = EWeaponType::EPistol;
	CurrentWeapon = Weapons[EWeaponType::EPistol];

	SetupTransformWeapon(TEXT("hand_rPistol"));
	ChangeWeaponMesh();
}

void UZeroInputNightComponent::SetShotgun()
{
	CurrentWeaponType = EWeaponType::EShotgun;
	CurrentWeapon = Weapons[EWeaponType::EShotgun];

	SetupTransformWeapon(TEXT("hand_rShotgun"));
	ChangeWeaponMesh();
}

void UZeroInputNightComponent::SetupTransformWeapon(const FName& SocketName)
{
	FVector Loc = Player->GetMesh()->GetSocketLocation(SocketName);
	FRotator Rot = Player->GetMesh()->GetSocketRotation(SocketName);
	CurrentWeapon->SetActorLocation(Loc);
	CurrentWeapon->SetActorRotation(Rot);
}

void UZeroInputNightComponent::ChangeWeaponMesh()
{
	FVector Loc = Player->GetMesh()->GetSocketLocation(TEXT("WeaponInventory"));
	FRotator Rot = Player->GetMesh()->GetSocketRotation(TEXT("WeaponInventory"));

	for (const auto& Weapon : Weapons)
	{
		if (Weapon.Value != Weapons[CurrentWeaponType])
		{
			Weapon.Value->SetActorLocation(Loc);
			Weapon.Value->SetActorRotation(Rot);
		}
	}
}

