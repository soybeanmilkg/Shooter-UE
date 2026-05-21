// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSettingRegistry.h"
#include "SGameSettingRegistry.generated.h"

#define GET_LOCAL_SETTINGS_FUNCTION_PATH(FunctionOrPropertyName)							\
	MakeShared<FGameSettingDataSourceDynamic>(TArray<FString>({								\
		GET_FUNCTION_NAME_STRING_CHECKED(USLocalPlayer, GetPlayerSaveData),					\
		GET_FUNCTION_NAME_STRING_CHECKED(USPlayerSaveData, FunctionOrPropertyName)			\
	}))

/**
 * 
 */
UCLASS()
class SHOOTER_API USGameSettingRegistry : public UGameSettingRegistry
{
	GENERATED_BODY()

public:
	//~ Begin UGameSettingRegistry
	virtual void OnInitialize(ULocalPlayer* InLocalPlayer) override;
	virtual void SaveChanges() override;
	//~ End UGameSettingRegistry
};
