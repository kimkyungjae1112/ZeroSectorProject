// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ZeroSoundManager.h"
#include "Sound/SoundBase.h"
#include "ZeroSoundManager.h"

void UZeroSoundManager::Init()
{
	PauseSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Cue_Ashorttactilesou3_Cue_Cue.Cue_Ashorttactilesou3_Cue_Cue"));
	ProvisoInteractSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Cue_Ashorttactilesou1_Cue_Cue.Cue_Ashorttactilesou1_Cue_Cue"));
	UIClickSFX = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Cue_Dampsquishysound5_Cue_Cue.Cue_Dampsquishysound5_Cue_Cue"));
}
