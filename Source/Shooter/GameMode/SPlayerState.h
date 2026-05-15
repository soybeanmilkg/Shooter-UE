// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "SPlayerState.generated.h"

class ASPlayerController;
class USPlayerSaveData;
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

public:
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category="Shooter", DisplayName="Get Player Name (Text)")
	FText GetPlayerName_Text() const;

	UFUNCTION(BlueprintCallable, Category="Shooter", DisplayName="Set Player Name (Text)")
	void SetPlayerName_Text(const FText InPlayerName);

private:
	UPROPERTY(Transient)
	TObjectPtr<ASPlayerController> OwningPlayer { nullptr };

	UPROPERTY(Transient)
	TObjectPtr<USPlayerSaveData> PlayerSaveData { nullptr };

	void LoadSaveData();
	void SaveData();
};
