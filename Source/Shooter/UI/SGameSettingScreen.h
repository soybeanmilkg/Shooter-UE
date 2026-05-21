// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/GameSettingScreen.h"
#include "SGameSettingScreen.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API USGameSettingScreen : public UGameSettingScreen
{
	GENERATED_BODY()

protected:
	//~ Begin UGameSettingScreen
	virtual UGameSettingRegistry* CreateRegistry() override;
	//~ End UGameSettingScreen
};
