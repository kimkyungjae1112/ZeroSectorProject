// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ZeroWaveTrigger.h"
#include "Components/BoxComponent.h"
#include "Interface/ZeroClassIdentifierInterface.h"
#include "Game/ZeroGameInstance.h"
#include "Game/ZeroSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ZeroGameModeBase.h"

AZeroWaveTrigger::AZeroWaveTrigger()
{
	BoxComp->OnComponentHit.AddDynamic(this, &AZeroWaveTrigger::OnHit);
}

void AZeroWaveTrigger::BeginPlay()
{
	Super::BeginPlay();

}

void AZeroWaveTrigger::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		IZeroClassIdentifierInterface* CII = Cast<IZeroClassIdentifierInterface>(OtherActor);
		if (CII && CII->GetClassName() == TEXT("Player"))
		{
			AZeroGameModeBase* GameMode = Cast<AZeroGameModeBase>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->StartWave();
				Destroy();
			}

			UZeroGameInstance* GI = Cast<UZeroGameInstance>(GetGameInstance());
			if (GI && GI->GetSoundManager() && GI->GetSoundManager()->TriggerSFX)
			{
				UGameplayStatics::PlaySound2D(this, GI->GetSoundManager()->TriggerSFX);
			}
		}
	}

	
}
