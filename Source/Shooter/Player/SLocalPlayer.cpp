// Fill out your copyright notice in the Description page of Project Settings.


#include "SLocalPlayer.h"

#include "SGameUserSettings.h"
#include "SPlayerSaveData.h"

USPlayerSaveData* USLocalPlayer::GetPlayerSaveData() const
{
	if (PlayerSaveData == nullptr)
	{
		// 是否可以在登录前加载存档
		// On PC it's okay to use the sync load because it only checks the disk
		// This could use a platform tag to check for proper save support instead
		constexpr bool bCanLoadBeforeLogin = PLATFORM_DESKTOP;

		if (bCanLoadBeforeLogin)
		{
			PlayerSaveData = USPlayerSaveData::LoadOrCreateSaveData(this);
		}
		else
		{
			PlayerSaveData = USPlayerSaveData::CreateTemporarySaveData(this);
		}
	}

	return PlayerSaveData;
}

USGameUserSettings* USLocalPlayer::GetGameUserSettings() const
{
	return USGameUserSettings::Get();
}

void USLocalPlayer::LoadPlayerSaveData(const bool bForceLoad)
{
	const FUniqueNetIdRepl CurrentNetId = GetCachedUniqueNetId();
	if (!bForceLoad && PlayerSaveData && CurrentNetId == NetIdForSaveData)
	{
		// 存档已加载
		return;
	}

	ensure(USPlayerSaveData::AsyncLoadOrCreateSaveData(this, FOnSPlayerSaveGameLoaded::CreateUObject(this, &ThisClass::OnLoadPlayerSaveData)));
}

void USLocalPlayer::OnLoadPlayerSaveData(USPlayerSaveData* InPlayerSaveData)
{
	if (ensure(InPlayerSaveData))
	{
		PlayerSaveData = InPlayerSaveData;
		NetIdForSaveData = GetCachedUniqueNetId();
	}
}
