// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Input/ZeroInputNightComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "ZeroHeader/ZeroWeaponHeader.h"
#include "UI/ZeroHUDWidget.h"
#include "UI/ZeroEnforceBoardWidget.h"
#include "Interface/ZeroNightInputInterface.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "ZeroSector.h"

FOnEnforceWeapon UZeroInputNightComponent::OnEnforceWeapon;

UZeroInputNightComponent::UZeroInputNightComponent()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> MoveTableRef(TEXT("/Script/Engine.DataTable'/Game/Data/Animation/ZeroPlayerAnimDataTable.ZeroPlayerAnimDataTable'"));
	if (MoveTableRef.Object)
	{
		MoveTable = MoveTableRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimationAsset> PistolFireRef(TEXT("/Script/Engine.AnimSequence'/Game/Animation/AS/MM_Pistol_Fire.MM_Pistol_Fire'"));
	if (PistolFireRef.Object)
	{
		PistolFire = PistolFireRef.Object;
	}

	CurrentWeaponType = EWeaponType::EPistol;
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
	switch (CurrentWeaponType)
	{
	case EWeaponType::EPistol:
		//Player->GetMesh()->PlayAnimation(PistolFire, false);
		//ZE_LOG(LogZeroSector, Display, TEXT("Pistol Fire"));
		break;
	case EWeaponType::ERifle:
		break;
	case EWeaponType::EShotgun:
		break;
	default:
		break;
	}

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
	//Anim->Montage_Play(Reloading);

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
		Weapons[EWeaponType::ERifle]->Upgrade(UZeroEnforceBoardWidget::RifleLevel);
		Weapons[EWeaponType::EPistol]->Upgrade(UZeroEnforceBoardWidget::PistolLevel);
		Weapons[EWeaponType::ERifle]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rRifle"));
		Weapons[EWeaponType::EPistol]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rPistol"));
		ChoicedWeapon = EWeaponType::ERifle;
		break;
	case EWeaponType::EShotgun:
		Weapons.Add(EWeaponType::EShotgun, GetWorld()->SpawnActor<AZeroWeaponShotgun>(AZeroWeaponShotgun::StaticClass()));
		Weapons.Add(EWeaponType::EPistol, GetWorld()->SpawnActor<AZeroWeaponPistol>(AZeroWeaponPistol::StaticClass()));
		Weapons[EWeaponType::EShotgun]->Upgrade(UZeroEnforceBoardWidget::ShotgunLevel);
		Weapons[EWeaponType::EPistol]->Upgrade(UZeroEnforceBoardWidget::PistolLevel);
		Weapons[EWeaponType::EShotgun]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rShotgun"));
		Weapons[EWeaponType::EPistol]->AttachToComponent(Player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rPistol"));
		ChoicedWeapon = EWeaponType::EShotgun;
		break;
	default:
		ZE_LOG(LogZeroSector, Error, TEXT("무기 안들어옴"));
		break;
	}


	IZeroNightInputInterface* Interface = Cast<IZeroNightInputInterface>(Player);
	if (Interface)
	{
		for (const auto& Weapon : Weapons)
		{
			Weapon.Value->StatApply();
			Weapon.Value->SetOwner(Player);
			Weapon.Value->OnSetMaxAmmo.BindUObject(Interface->GetWeaponHUDWidget(), &UZeroHUDWidget::UpdateMaxAmmo);
			Weapon.Value->OnChangedAmmo.BindUObject(Interface->GetWeaponHUDWidget(), &UZeroHUDWidget::UpdateCurrentAmmo);
		}
	}


	SetPistol();
}

void UZeroInputNightComponent::SetUnequipWeapon()
{
	SetNoWeapon();
}

EWeaponType UZeroInputNightComponent::GetWeaponType()
{
	return CurrentWeaponType;
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
	Anim = Player->GetMesh()->GetAnimInstance();
	ensure(Anim);

	MontageData = *MoveTable->FindRow<FZeroPlayerAnimDataTable>(TEXT("Player"), FString());
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
	CurrentWeapon->GunAmmoTextDisplay();
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

UAnimMontage* UZeroInputNightComponent::GetPistolFireMontage() const
{
	ZE_LOG(LogZeroSector, Display, TEXT("Pistol Montage"));
	if (MontageData.PistolAnim.FireMontage.IsPending())
	{
		MontageData.PistolAnim.FireMontage.LoadSynchronous();
	}
	return MontageData.PistolAnim.FireMontage.Get();
}

UAnimMontage* UZeroInputNightComponent::GetPistolReloadingMontage() const
{
	if (MontageData.PistolAnim.ReloadingMontage.IsPending())
	{
		MontageData.PistolAnim.ReloadingMontage.LoadSynchronous();
	}
	return MontageData.PistolAnim.ReloadingMontage.Get();
}

UAnimMontage* UZeroInputNightComponent::GetRifleFireMontage() const
{
	if (MontageData.RifleAnim.FireMontage.IsPending())
	{
		MontageData.RifleAnim.FireMontage.LoadSynchronous();
	}
	return MontageData.RifleAnim.FireMontage.Get();
}

UAnimMontage* UZeroInputNightComponent::GetRifleReloadingMontage() const
{
	if (MontageData.RifleAnim.ReloadingMontage.IsPending())
	{
		MontageData.RifleAnim.ReloadingMontage.LoadSynchronous();
	}
	return MontageData.RifleAnim.ReloadingMontage.Get();
}

UAnimMontage* UZeroInputNightComponent::GetShotgunFireMontage() const
{
	if (MontageData.ShotgunAnim.FireMontage.IsPending())
	{
		MontageData.ShotgunAnim.FireMontage.LoadSynchronous();
	}
	return MontageData.ShotgunAnim.FireMontage.Get();
}

UAnimMontage* UZeroInputNightComponent::GetShotgunReloadingMontage() const
{
	if (MontageData.ShotgunAnim.ReloadingMontage.IsPending())
	{
		MontageData.ShotgunAnim.ReloadingMontage.LoadSynchronous();
	}
	return MontageData.ShotgunAnim.ReloadingMontage.Get();
}

UAnimMontage* UZeroInputNightComponent::GetDeadMontage() const
{
	if (MontageData.DeadMontage.IsPending())
	{
		MontageData.DeadMontage.LoadSynchronous();
	}
	return MontageData.DeadMontage.Get();
}
