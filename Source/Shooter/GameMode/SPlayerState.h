// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "NativeGameplayTags.h"
#include "SPlayerState.generated.h"

class ASPlayerController;
class USPlayerSaveData;

USTRUCT(BlueprintType)
struct FSPlayerStateReadyMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class ASPlayerState> PlayerState { nullptr };
};

namespace Shooter::Tags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PlayerStateReadyMessage);
}

/**
 * 
 */
UCLASS()
class SHOOTER_API ASPlayerState : public AModularPlayerState
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

	//~ Begin PlayerState
	virtual void OnRep_PlayerId() override;
	//~ End PlayerState

public:
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="Shooter", DisplayName="Get Player Name (Text)")
	FText GetPlayerName_Text() const;

	UFUNCTION(BlueprintCallable, Category="Shooter", DisplayName="Set Player Name (Text)")
	void SetPlayerName_Text(const FText InPlayerName);

	void BroadcastPlayerStateReady();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Shooter")
	void ChangeScore(const int32 Value);

private:
	UPROPERTY(Transient)
	TObjectPtr<ASPlayerController> OwningPlayer { nullptr };

	UPROPERTY(Transient)
	TObjectPtr<USPlayerSaveData> PlayerSaveData { nullptr };

	UPROPERTY(Replicated)
	FText PlayerDisplayName {};

	UFUNCTION(Server, Reliable)
	void SetPlayerDisplayName(const FText& InDisplayName);

	void LoadSaveData();
	void SaveData();
};
