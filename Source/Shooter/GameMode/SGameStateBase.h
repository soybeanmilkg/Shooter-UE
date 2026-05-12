// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterSaveData.h"
#include "GameFramework/GameStateBase.h"
#include "SGameStateBase.generated.h"

class ASPlayerController;
class USServerSaveData;
/**
 * 
 */
UCLASS()
class SHOOTER_API ASGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

protected:
	//~ Begin AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor

	//~ Begin UObject
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End UObject

public:
	// 获取分数信息，已排序
	UFUNCTION(BlueprintPure, Category="Shooter")
	TArray<FSPlayerScoreInfo> GetScoreInfos() const;

	// 记录玩家分数信息
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Shooter")
	void RecordScoreInfo(ASPlayerController* PlayerController);

protected:
	// 背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	TObjectPtr<USoundBase> BackgroundMusic { nullptr };

	// 服务器存档数据
	UPROPERTY(Transient)
	TObjectPtr<USServerSaveData> ServerSaveData { nullptr };

	// 分数信息
	UPROPERTY(Replicated)
	TArray<FSPlayerScoreInfo> ScoreInfos {};

	void LoadSaveData();
	void SaveData();
};
