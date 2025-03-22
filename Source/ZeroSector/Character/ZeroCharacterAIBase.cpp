// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ZeroCharacterAIBase.h"               
#include "AIController.h"                         
#include "BehaviorTree/BlackboardComponent.h"     
#include "BrainComponent.h"                       
#include "TimerManager.h" 

AZeroCharacterAIBase::AZeroCharacterAIBase()
{
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -85.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
}



void AZeroCharacterAIBase::BeginPlay()
{
    Super::BeginPlay();

   
    if (AAIController* AICon = Cast<AAIController>(GetController()))
    {
        if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
        {
            BB->SetValueAsBool("bIsRanged", bIsRangedZombie);
            BB->SetValueAsFloat("AttackRange", bIsRangedZombie ? 1000.f : 200.f); 
        }
    } 
}

void AZeroCharacterAIBase::ApplyDamage(float Damage)
{
    Health -= Damage;

    if (Health <= 0.f)
    {
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                BB->SetValueAsBool("bIsDead", true);
            }

        }

    }
    else
    {
        if (AAIController* AICon = Cast<AAIController>(GetController()))
        {
            if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
            {
                BB->SetValueAsBool("bIsHit", true);
                UE_LOG(LogTemp, Warning, TEXT("attacked"));

                GetWorldTimerManager().SetTimer(HitResetTimerHandle, [BB]()
                    {
                        BB->SetValueAsBool("bIsHit", false);
                    }, 0.4f, false);
            }
        }

    }
}