// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SPlayerSaveData.generated.h"

class USLocalPlayer;

DECLARE_DELEGATE_OneParam(FOnSPlayerSaveGameLoaded, class USPlayerSaveData*);

/**
 * 
 */
UCLASS()
class SHOOTER_API USPlayerSaveData : public ULocalPlayerSaveGame
{
	GENERATED_BODY()

public:
	static USPlayerSaveData* LoadOrCreateSaveData(const USLocalPlayer* LocalPlayer);

	static USPlayerSaveData* CreateTemporarySaveData(const USLocalPlayer* LocalPlayer);

	static bool AsyncLoadOrCreateSaveData(const USLocalPlayer* LocalPlayer, FOnSPlayerSaveGameLoaded Delegate);

	//~ Begin ULocalPlayerSaveGame
	virtual void ResetToDefault() override;
	//~ End ULocalPlayerSaveGame

	void SaveData();

public:
	UFUNCTION()
	FString GetPlayerName() { return PlayerName; }

	UFUNCTION()
	void SetPlayerName(const FString& InPlayerName) { PlayerName = InPlayerName; }

private:
	UPROPERTY(SaveGame)
	FString PlayerName { TEXT("Shooter") };
};
