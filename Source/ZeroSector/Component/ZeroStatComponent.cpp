// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/ZeroStatComponent.h"
#include "Data/ZeroSingleton.h"
#include "Interface/ZeroClassIdentifierInterface.h"
#include "ZeroSector.h"

UZeroStatComponent::UZeroStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UZeroStatComponent::BeginPlay()
{
	Super::BeginPlay();

	IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(GetOwner());
	if (CII)
	{
		SetStat(CII->GetClassName());
		CurrentHp = BaseStat.MaxHp;
	}
}


void UZeroStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UZeroStatComponent::SetStat(const FName& InName)
{
	BaseStat = UZeroSingleton::Get().GetCharacterStat(InName);
	check(BaseStat.MaxHp > 0.f);
}

float UZeroStatComponent::ApplyDamage(float InDamage)
{
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage);
	const float PrevHp = CurrentHp;

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return 0.0f;
}

void UZeroStatComponent::SetHp(float NewHp)
{
	const float ActualHp = FMath::Clamp(NewHp, 0, BaseStat.MaxHp);
	CurrentHp = NewHp;
	OnHpChanged.Broadcast(CurrentHp);
	ZE_LOG(LogZeroSector, Display, TEXT("Rest Health : %f"), CurrentHp);
}

