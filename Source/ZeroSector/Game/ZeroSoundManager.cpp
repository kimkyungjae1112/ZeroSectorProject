// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroSoundManager.h"
#include "Sound/SoundBase.h"
#include "ZeroSoundManager.h"

void UZeroSoundManager::Init()
{
	PauseSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Cue_Ashorttactilesou3_Cue_Cue.Cue_Ashorttactilesou3_Cue_Cue"));
	ProvisoInteractSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Cue_Ashorttactilesou1_Cue_Cue.Cue_Ashorttactilesou1_Cue_Cue"));
	UIClickSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/UIClick.UIClick"));
	NoteSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Note.Note"));
	OperationSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Operation.Operation"));
	ExcludeSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Exclude.Exclude"));
	SelectSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Select.Select"));
	EnforceSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Enforce.Enforce"));
	ReloadSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Reload.Reload"));
	ChangeWeaponSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ChangeWeapon.ChangeWeapon"));
	Ammo0SFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Ammo0.Ammo0"));
	TriggerSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Trigger.Trigger"));
	FootstepSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Footstep.Footstep"));
	ZombieBiteSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ZombieBite.ZombieBite"));
	ZombieMoveSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ZombieMove.ZombieMove"));
	ZombieDieSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ZombieDie.ZombieDie"));

}
