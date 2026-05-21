// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameInstance.h"

#include "CommonUserSubsystem.h"
#include "Player/SLocalPlayer.h"

void USGameInstance::HandlerUserInitialized(const UCommonUserInfo* UserInfo, bool bSuccess, FText Error, ECommonUserPrivilege RequestedPrivilege, ECommonUserOnlineContext OnlineContext)
{
	Super::HandlerUserInitialized(UserInfo, bSuccess, Error, RequestedPrivilege, OnlineContext);

	if (bSuccess && ensure(UserInfo))
	{
		if (USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(GetLocalPlayerByIndex(UserInfo->LocalPlayerIndex)))
		{
			LocalPlayer->LoadPlayerSaveData();
		}
	}
}
