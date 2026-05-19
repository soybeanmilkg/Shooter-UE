// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameState.h"
#include "ShooterSaveData.h"
#include "SGameStateBase.generated.h"

class UGameMainAsset;
class UGameExperienceManagerComponent;
class ASPlayerController;
class USServerSaveData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerIdsChangedDelegate);

/**
 * 
 */
UCLASS()
class SHOOTER_API ASGameStateBase : public AModularGameState
{
	GENERATED_BODY()

public:
	explicit ASGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ Begin AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor

	//~ Begin UObject
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End UObject

public:
	// 获取排行榜分数信息，已排序
	UFUNCTION(BlueprintPure, Category="Shooter")
	TArray<FSPlayerScoreInfo> GetScoreInfos() const;

	// 记录玩家分数信息
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Shooter")
	void RecordScoreInfo(ASPlayerController* PlayerController);

	UGameExperienceManagerComponent* GetExperienceManager() const { return ExperienceManager; }

	void AddPlayerId(const int32 PlayerId);
	void RemovePlayerId(const int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category="Shooter")
	TArray<int32> GetPlayerIds() const { return PlayerIds; }

	UFUNCTION(BlueprintCallable, Category="Shooter", meta=(WorldContext="WorldContextObject"))
	static APlayerState* GetPlayerStateByPlayerId(UObject* WorldContextObject, const int32 PlayerId);

protected:
	UPROPERTY(Transient)
	TObjectPtr<UGameExperienceManagerComponent> ExperienceManager { nullptr };

	// 背景音乐
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	TObjectPtr<USoundBase> BackgroundMusic { nullptr };

	// 服务器存档数据
	UPROPERTY(Transient)
	TObjectPtr<USServerSaveData> ServerSaveData { nullptr };

	// 排行榜分数信息
	UPROPERTY(Replicated)
	TArray<FSPlayerScoreInfo> ScoreInfos {};

	UPROPERTY(ReplicatedUsing=OnRep_PlayerIds)
	TArray<int32> PlayerIds {};

	UFUNCTION()
	void OnRep_PlayerIds();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerIdsChangedDelegate OnPlayerIdsChanged {};

	void LoadSaveData();
	void SaveData();
};
