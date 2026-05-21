// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSettingValue_String.h"

#include "DataSource/GameSettingDataSource.h"

static constexpr uint8 GAME_SETTING_GETTER_FLAG { 1 << 0 };
static constexpr uint8 GAME_SETTING_SETTER_FLAG { 1 << 1 };
static constexpr uint8 GAME_SETTING_STARTUP_FLAGS { GAME_SETTING_GETTER_FLAG | GAME_SETTING_SETTER_FLAG };

void USGameSettingValue_String::Startup()
{
	StartupFlags = 0;
	Getter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnDataSourcesReady, GAME_SETTING_GETTER_FLAG));
	Setter->Startup(LocalPlayer, FSimpleDelegate::CreateUObject(this, &ThisClass::OnDataSourcesReady, GAME_SETTING_SETTER_FLAG));
}

void USGameSettingValue_String::StoreInitial()
{
	InitialValue = GetValue();
}

void USGameSettingValue_String::ResetToDefault()
{
	if (DefaultValue.IsSet())
	{
		SetValue(DefaultValue.GetValue(), EGameSettingChangeReason::ResetToDefault);
	}
}

void USGameSettingValue_String::RestoreToInitial()
{
	SetValue(InitialValue, EGameSettingChangeReason::RestoreToInitial);
}

FString USGameSettingValue_String::GetAnalyticsValue() const
{
	return GetValue();
}

TOptional<FString> USGameSettingValue_String::GetDefaultValue() const
{
	return DefaultValue;
}

void USGameSettingValue_String::SetValue(const FString& Value, EGameSettingChangeReason Reason)
{
	Setter->SetValue(LocalPlayer, Value);

	NotifySettingChanged(Reason);
}

FString USGameSettingValue_String::GetValue() const
{
	return Getter->GetValueAsString(LocalPlayer);
}

void USGameSettingValue_String::SetDynamicGetter(const TSharedRef<FGameSettingDataSource>& InGetter)
{
	Getter = InGetter;
}

void USGameSettingValue_String::SetDynamicSetter(const TSharedRef<FGameSettingDataSource>& InSetter)
{
	Setter = InSetter;
}

void USGameSettingValue_String::SetDefaultValue(const FString& InValue)
{
	DefaultValue = InValue;
}

void USGameSettingValue_String::OnInitialized()
{
#if !UE_BUILD_SHIPPING
	ensureAlways(Getter);
	ensureAlwaysMsgf(Getter->Resolve(LocalPlayer), TEXT("%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties?"), *GetDevName().ToString(), *Getter->ToString());
	ensureAlways(Setter);
	ensureAlwaysMsgf(Setter->Resolve(LocalPlayer), TEXT("%s: %s did not resolve, are all functions and properties valid, and are they UFunctions/UProperties?"), *GetDevName().ToString(), *Setter->ToString());
#endif

	Super::OnInitialized();
}

void USGameSettingValue_String::OnDataSourcesReady(const uint8 Flag)
{
	StartupFlags |= Flag;
	if (StartupFlags == GAME_SETTING_STARTUP_FLAGS)
	{
		StartupComplete();
	}
}
