// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	explicit ASGameModeBase(const FObjectInitializer& ObjectInitializer);
};
