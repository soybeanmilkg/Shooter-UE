// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameSettingScreen.h"

#include "Settings/SGameSettingRegistry.h"

UGameSettingRegistry* USGameSettingScreen::CreateRegistry()
{
	const auto NewRegistry = NewObject<USGameSettingRegistry>();

	NewRegistry->Initialize(GetOwningLocalPlayer());

	return NewRegistry;
}
