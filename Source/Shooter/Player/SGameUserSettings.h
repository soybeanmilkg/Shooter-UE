// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SGameUserSettings.generated.h"

class USoundControlBus;
class USoundControlBusMix;
/**
 * 
 */
UCLASS()
class SHOOTER_API USGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static USGameUserSettings* Get();

	UFUNCTION()
	FORCEINLINE float GetOverallVolume() const { return OverallVolume; }

	UFUNCTION()
	void SetOverallVolume(const float InVolume);

	UFUNCTION()
	FORCEINLINE float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION()
	void SetMusicVolume(const float InVolume);

	UFUNCTION()
	FORCEINLINE float GetSFXVolume() const { return SFXVolume; }

	UFUNCTION()
	void SetSFXVolume(const float InVolume);

	UFUNCTION()
	FORCEINLINE float GetUIVolume() const { return UIVolume; }

	UFUNCTION()
	void SetUIVolume(const float InVolume);

private:
	UPROPERTY(Config)
	float OverallVolume { 1.0f };
	UPROPERTY(Config)
	float MusicVolume { 1.0f };
	UPROPERTY(Config)
	float SFXVolume { 1.0f };
	UPROPERTY(Config)
	float UIVolume { 1.0f };
	void SetVolumeForControlBus(const FName& Name, const float InVolume);
};
