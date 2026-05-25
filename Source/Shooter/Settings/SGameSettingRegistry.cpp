// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSettingRegistry.h"

#include "GameSettingCollection.h"
#include "GameSettingValueScalarDynamic.h"
#include "CustomSettings/SGameSettingValue_String.h"
#include "DataSource/GameSettingDataSourceDynamic.h"
#include "Player/SGameUserSettings.h"
#include "Player/SLocalPlayer.h"
#include "Player/SPlayerSaveData.h"

#define LOCTEXT_NAMESPACE "Shooter"

void USGameSettingRegistry::OnInitialize(ULocalPlayer* InLocalPlayer)
{
	USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(InLocalPlayer);

	UGameSettingCollection* Settings = NewObject<UGameSettingCollection>();
	Settings->SetDevName("ShooterSettings");
	Settings->SetDisplayName(LOCTEXT("太空战机设置", "太空战机设置"));
	Settings->Initialize(LocalPlayer);

	{
		USGameSettingValue_String* Setting = NewObject<USGameSettingValue_String>();
		Setting->SetDevName("PlayerName");
		Setting->SetDisplayName(LOCTEXT("玩家名称", "玩家名称"));
		Setting->SetDescriptionRichText(LOCTEXT("设置玩家名称", "设置玩家名称。"));

		Setting->SetDynamicGetter(GET_SAVE_DATA_SETTINGS_FUNCTION_PATH(GetPlayerName));
		Setting->SetDynamicSetter(GET_SAVE_DATA_SETTINGS_FUNCTION_PATH(SetPlayerName));
		Setting->SetDefaultValue(TEXT("Shooter"));

		Settings->AddSetting(Setting);
	}

	{
		UGameSettingValueScalarDynamic* Setting = NewObject<UGameSettingValueScalarDynamic>();
		Setting->SetDevName("OverallVolume");
		Setting->SetDisplayName(LOCTEXT("音量", "音量"));
		Setting->SetDescriptionRichText(LOCTEXT("设置音量", "设置音量。"));

		Setting->SetDynamicGetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(GetOverallVolume));
		Setting->SetDynamicSetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(SetOverallVolume));
		Setting->SetDefaultValue(GetDefault<USGameUserSettings>()->GetOverallVolume());
		Setting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

		Settings->AddSetting(Setting);
	}

	{
		UGameSettingValueScalarDynamic* Setting = NewObject<UGameSettingValueScalarDynamic>();
		Setting->SetDevName("MusicVolume");
		Setting->SetDisplayName(LOCTEXT("音乐音量", "音乐音量"));
		Setting->SetDescriptionRichText(LOCTEXT("设置音乐音量", "设置音乐音量。"));

		Setting->SetDynamicGetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(GetMusicVolume));
		Setting->SetDynamicSetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(SetMusicVolume));
		Setting->SetDefaultValue(GetDefault<USGameUserSettings>()->GetMusicVolume());
		Setting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

		Settings->AddSetting(Setting);
	}

	{
		UGameSettingValueScalarDynamic* Setting = NewObject<UGameSettingValueScalarDynamic>();
		Setting->SetDevName("SFXVolume");
		Setting->SetDisplayName(LOCTEXT("音效音量", "音效音量"));
		Setting->SetDescriptionRichText(LOCTEXT("设置音效音量", "设置音效音量。"));

		Setting->SetDynamicGetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(GetSFXVolume));
		Setting->SetDynamicSetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(SetSFXVolume));
		Setting->SetDefaultValue(GetDefault<USGameUserSettings>()->GetSFXVolume());
		Setting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

		Settings->AddSetting(Setting);
	}

	{
		UGameSettingValueScalarDynamic* Setting = NewObject<UGameSettingValueScalarDynamic>();
		Setting->SetDevName("UIVolume");
		Setting->SetDisplayName(LOCTEXT("UI音量", "UI音量"));
		Setting->SetDescriptionRichText(LOCTEXT("设置UI音量", "设置UI音量。"));

		Setting->SetDynamicGetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(GetUIVolume));
		Setting->SetDynamicSetter(GET_GAME_USER_SETTINGS_FUNCTION_PATH(SetUIVolume));
		Setting->SetDefaultValue(GetDefault<USGameUserSettings>()->GetUIVolume());
		Setting->SetDisplayFormat(UGameSettingValueScalarDynamic::ZeroToOnePercent);

		Settings->AddSetting(Setting);
	}

	RegisterSetting(Settings);
}

void USGameSettingRegistry::SaveChanges()
{
	Super::SaveChanges();

	if (const USLocalPlayer* LocalPlayer = Cast<USLocalPlayer>(OwningLocalPlayer))
	{
		// Game user settings need to be applied to handle things like resolution, this saves indirectly
		LocalPlayer->GetGameUserSettings()->ApplySettings(false);

		LocalPlayer->GetPlayerSaveData()->SaveData();
	}
}

#undef LOCTEXT_NAMESPACE
