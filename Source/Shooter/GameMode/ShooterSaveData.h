// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShooterSaveData.generated.h"

namespace Shooter::SaveGame
{
	extern FString Slot_Server;
	extern FString Slot_Player;
}

USTRUCT(BlueprintType)
struct FSPlayerScoreInfo
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FText PlayerName {};

	UPROPERTY(SaveGame, BlueprintReadWrite)
	float Score { 0 };
};

/**
 * 服务器数据存档
 */
UCLASS()
class SHOOTER_API USServerSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	TArray<FSPlayerScoreInfo> ScoreInfos {};
};

/**
 * 本地玩家存档
 */
UCLASS()
class SHOOTER_API USPlayerSaveData : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame)
	FText PlayerName { FText::FromString("Shooter") };
};
