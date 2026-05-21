// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "CustomSettings/SGameSettingValue_String.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "Player/SLocalPlayer.h"
#include "Player/SPlayerSaveData.h"

#define LOCTEXT_NAMESPACE "Shooter"

void USGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(InLocalPlayer);

	UGameSettingCollection* PlayerInfoSettings = NewObject<UGameSettingCollection>();
	PlayerInfoSettings->SetDevName("PlayerInfoSettings");
	PlayerInfoSettings->SetDisplayName(LOCTEXT("玩家信息设置", "玩家信息设置"));
	PlayerInfoSettings->Initialize(LocalPlayer);

	{
		USGameSettingValue_String* PlayerNameSetting = NewObject<USGameSettingValue_String>();
		PlayerNameSetting->SetDevName("PlayerName");
		PlayerNameSetting->SetDisplayName(LOCTEXT("玩家名称", "玩家名称"));
		PlayerNameSetting->SetDescriptionRichText(LOCTEXT("设置玩家名称", "设置玩家名称。"));

		PlayerNameSetting->SetDynamicGetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(GetPlayerName));
		PlayerNameSetting->SetDynamicSetter(GET_LOCAL_SETTINGS_FUNCTION_PATH(SetPlayerName));
		PlayerNameSetting->SetDefaultValue(TEXT("Shooter"));

		PlayerInfoSettings->AddSetting(PlayerNameSetting);
	}

	RegisterSetting(PlayerInfoSettings);
}

void USGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();

	if (USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		// LocalPlayer->GetLocalSettings()->ApplySettings(false);

		LocalPlayer->GetPlayerSaveData()->SaveData();
	}
}

#undef LOCTEXT_NAMESPACE
