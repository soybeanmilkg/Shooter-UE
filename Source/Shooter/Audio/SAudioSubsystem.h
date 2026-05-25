// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SAudioSubsystem.generated.h"

class USoundControlBus;
class USoundControlBusMix;
/**
 * 
 */
UCLASS()
class SHOOTER_API USAudioSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

protected:
	//~ Begin UWorldSubsystem
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	//~ End UWorldSubsystem

public:
	USoundControlBusMix* GetUserControlBusMix() const { return UserControlBusMix; }

	USoundControlBus* GetUserControlBus(const FName& Name) const;

private:
	bool bSoundControlBusMixLoaded { false };

	UPROPERTY(Transient)
	TObjectPtr<USoundControlBusMix> UserControlBusMix { nullptr };

	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<USoundControlBus>> UserControlBusMap {};

	void LoadUserControlBusMix();
};
