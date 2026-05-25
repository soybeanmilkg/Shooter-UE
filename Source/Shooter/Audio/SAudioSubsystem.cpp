// Fill out your copyright notice in the Description page of Project Settings.


#include "SAudioSubsystem.h"

#include "AudioModulationStatics.h"
#include "SAudioSettings.h"
#include "SoundControlBus.h"
#include "SoundControlBusMix.h"
#include "Player/SGameUserSettings.h"

void USAudioSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	LoadUserControlBusMix();
}

USoundControlBus* USAudioSubsystem::GetUserControlBus(const FName& Name) const
{
	if (const TObjectPtr<USoundControlBus>* ControlBusPtr = UserControlBusMap.Find(Name))
	{
		return *ControlBusPtr;
	}

	return nullptr;
}

void USAudioSubsystem::LoadUserControlBusMix()
{
	UWorld* World = GEngine ? GEngine->GetCurrentPlayWorld() : nullptr;
	const USAudioSettings* AudioSettings = GetDefault<USAudioSettings>();
	const USGameUserSettings* GameUserSettings = USGameUserSettings::Get();

	if (World == nullptr || AudioSettings == nullptr || AudioSettings == nullptr) return;

	TArray<FSoundControlBusMixStage> ControlBusMixStageArray {};

	auto LoadSoundControlBus = [this, World, &ControlBusMixStageArray](const FName& Name, const FSoftObjectPath& SoundControlBusPath, const float Volume)
	{
		if (USoundControlBus* SoundControlBus = Cast<USoundControlBus>(SoundControlBusPath.TryLoad()))
		{
			UserControlBusMap.Emplace(Name, SoundControlBus);
			const FSoundControlBusMixStage Stage = UAudioModulationStatics::CreateBusMixStage(World, SoundControlBus, Volume);
			ControlBusMixStageArray.Emplace(Stage);
		}
		else
		{
			ensureMsgf(false, TEXT("%s Control Bus reference missing from Shooter Audio Settings."), *Name.ToString());
		}
	};

	LoadSoundControlBus(TEXT("Overall"), AudioSettings->OverallVolumeControlBus, GameUserSettings->GetOverallVolume());
	LoadSoundControlBus(TEXT("Music"), AudioSettings->MusicVolumeControlBus, GameUserSettings->GetMusicVolume());
	LoadSoundControlBus(TEXT("SFX"), AudioSettings->SFXVolumeControlBus, GameUserSettings->GetSFXVolume());
	LoadSoundControlBus(TEXT("UI"), AudioSettings->UIVolumeControlBus, GameUserSettings->GetUIVolume());

	if (USoundControlBusMix* SoundControlBusMix = Cast<USoundControlBusMix>(AudioSettings->UserSettingsControlBusMix.TryLoad()))
	{
		UserControlBusMix = SoundControlBusMix;

		UAudioModulationStatics::ActivateBusMix(World, UserControlBusMix);

		UAudioModulationStatics::UpdateMix(World, UserControlBusMix, ControlBusMixStageArray);

		bSoundControlBusMixLoaded = true;
	}
	else
	{
		ensureMsgf(false, TEXT("User Settings Control Bus Mix reference missing from Shooter Audio Settings."));
	}
}
