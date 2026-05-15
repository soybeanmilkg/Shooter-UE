// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "SWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ASWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	FPrimaryAssetId GetGameExperienceAssetId() const { return ExperienceId; }

private:
	UPROPERTY(EditDefaultsOnly, Category="GameMode", meta=(AllowedClasses="/Script/GameExperience.GameExperienceDefinition"))
	FPrimaryAssetId ExperienceId {};
};
