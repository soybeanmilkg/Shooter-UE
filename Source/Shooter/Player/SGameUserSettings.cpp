// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameUserSettings.h"

#include "AudioModulationStatics.h"
#include "SoundControlBus.h"
#include "SoundControlBusMix.h"
#include "Audio/SAudioSettings.h"
#include "Audio/SAudioSubsystem.h"

USGameUserSettings* USGameUserSettings::Get()
{
	return GEngine ? Cast<USGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

void USGameUserSettings::SetOverallVolume(const float InVolume)
{
	OverallVolume = InVolume;
	SetVolumeForControlBus(TEXT("Overall"), OverallVolume);
}

void USGameUserSettings::SetMusicVolume(const float InVolume)
{
	MusicVolume = InVolume;
	SetVolumeForControlBus(TEXT("Music"), MusicVolume);
}

void USGameUserSettings::SetSFXVolume(const float InVolume)
{
	SFXVolume = InVolume;
	SetVolumeForControlBus(TEXT("SFX"), SFXVolume);
}

void USGameUserSettings::SetUIVolume(const float InVolume)
{
	UIVolume = InVolume;
	SetVolumeForControlBus(TEXT("UI"), UIVolume);
}

void USGameUserSettings::SetVolumeForControlBus(const FName& Name, const float InVolume)
{
	const UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr;
	if (World == nullptr) return;

	if (const USAudioSubsystem* AudioSubsystem = World->GetSubsystem<USAudioSubsystem>())
	{
		USoundControlBusMix* ControlBusMix = AudioSubsystem->GetUserControlBusMix();
		USoundControlBus* ControlBus = AudioSubsystem->GetUserControlBus(Name);

		if (ControlBusMix && ControlBus)
		{
			// Create and set the Control Bus Mix Stage Parameters
			FSoundControlBusMixStage UpdatedControlBusMixStage;
			UpdatedControlBusMixStage.Bus = ControlBus;
			UpdatedControlBusMixStage.Value.TargetValue = InVolume;
			UpdatedControlBusMixStage.Value.AttackTime = 0.01f;
			UpdatedControlBusMixStage.Value.ReleaseTime = 0.01f;

			// Add the Control Bus Mix Stage to an Array as the UpdateMix function requires
			TArray<FSoundControlBusMixStage> UpdatedMixStageArray;
			UpdatedMixStageArray.Add(UpdatedControlBusMixStage);

			// Modify the matching bus Mix Stage parameters on the User Control Bus Mix
			UAudioModulationStatics::UpdateMix(World, ControlBusMix, UpdatedMixStageArray);
		}
	}
}
