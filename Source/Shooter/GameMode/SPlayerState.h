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
	void BroadcastPlayerStateReady();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Shooter")
	void ChangeScore(const int32 Value);

private:
	UPROPERTY(Transient)
	TObjectPtr<ASPlayerController> OwningPlayer { nullptr };

	UPROPERTY(Replicated)
	FText PlayerDisplayName {};
};
