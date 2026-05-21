// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerSaveData.h"
#include "SLocalPlayer.h"

static FString PLAYER_SAVE_DATA_SLOT_NAME = TEXT("Player");

USPlayerSaveData* USPlayerSaveData::LoadOrCreateSaveData(const USLocalPlayer* LocalPlayer)
{
	auto SaveData = Cast<USPlayerSaveData>(LoadOrCreateSaveGameForLocalPlayer(StaticClass(), LocalPlayer, PLAYER_SAVE_DATA_SLOT_NAME));

	return SaveData;
}

USPlayerSaveData* USPlayerSaveData::CreateTemporarySaveData(const USLocalPlayer* LocalPlayer)
{
	auto SaveData = Cast<USPlayerSaveData>(CreateNewSaveGameForLocalPlayer(StaticClass(), LocalPlayer, PLAYER_SAVE_DATA_SLOT_NAME));

	return SaveData;
}

bool USPlayerSaveData::AsyncLoadOrCreateSaveData(const USLocalPlayer* LocalPlayer, FOnSPlayerSaveGameLoaded Delegate)
{
	return AsyncLoadOrCreateSaveGameForLocalPlayer(StaticClass(), LocalPlayer, PLAYER_SAVE_DATA_SLOT_NAME, FOnLocalPlayerSaveGameLoadedNative::CreateLambda([Delegate](ULocalPlayerSaveGame* SaveGame)
	{
		auto SaveData = Cast<USPlayerSaveData>(SaveGame);

		Delegate.ExecuteIfBound(SaveData);
	}));
}

void USPlayerSaveData::ResetToDefault()
{
	Super::ResetToDefault();

	PlayerName = TEXT("Shooter");
}

void USPlayerSaveData::SaveData()
{
	AsyncSaveGameToSlotForLocalPlayer();
}
