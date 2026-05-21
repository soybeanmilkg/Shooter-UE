// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "SLocalPlayer.generated.h"

class USPlayerSaveData;
/**
 * 
 */
UCLASS()
class SHOOTER_API USLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()

public:
	UFUNCTION()
	USPlayerSaveData* GetPlayerSaveData() const;

	void LoadPlayerSaveData(const bool bForceLoad = false);

private:
	UPROPERTY(Transient)
	mutable TObjectPtr<USPlayerSaveData> PlayerSaveData { nullptr };

	FUniqueNetIdRepl NetIdForSaveData {};

	void OnLoadPlayerSaveData(USPlayerSaveData* InPlayerSaveData);
};
