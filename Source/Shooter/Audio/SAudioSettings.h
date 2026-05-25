// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SAudioSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Shooter Audio Settings"))
class SHOOTER_API USAudioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, Category=UserMixSettings, meta=(AllowedClasses="/Script/AudioModulation.SoundControlBusMix"))
	FSoftObjectPath UserSettingsControlBusMix;

	UPROPERTY(Config, EditAnywhere, Category=UserMixSettings, meta=(AllowedClasses="/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath OverallVolumeControlBus;

	UPROPERTY(Config, EditAnywhere, Category=UserMixSettings, meta=(AllowedClasses="/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath MusicVolumeControlBus;

	UPROPERTY(Config, EditAnywhere, Category=UserMixSettings, meta=(AllowedClasses="/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath SFXVolumeControlBus;

	UPROPERTY(Config, EditAnywhere, Category=UserMixSettings, meta=(AllowedClasses="/Script/AudioModulation.SoundControlBus"))
	FSoftObjectPath UIVolumeControlBus;
};
