// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameModeBase.h"
#include "SGameModeBase.generated.h"

class UGameExperienceDefinition;

/**
 * 
 */
UCLASS()
class SHOOTER_API ASGameModeBase : public AModularGameModeBase
{
	GENERATED_BODY()

public:
	explicit ASGameModeBase(const FObjectInitializer& ObjectInitializer);

protected:
	//~ Begin AGameModeBase
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	//~ End GameModeBase

private:
	void HandleExperience() const;
};
