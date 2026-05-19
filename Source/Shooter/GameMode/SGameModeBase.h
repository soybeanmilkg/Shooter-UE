// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameMode.h"
#include "SGameModeBase.generated.h"

class UGameExperienceDefinition;

/**
 * 
 */
UCLASS()
class SHOOTER_API ASGameModeBase : public AModularGameMode
{
	GENERATED_BODY()

public:
	explicit ASGameModeBase(const FObjectInitializer& ObjectInitializer);

protected:
	//~ Begin AGameModeBase
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	//~ End GameModeBase

private:
	void HandleExperience() const;
};
