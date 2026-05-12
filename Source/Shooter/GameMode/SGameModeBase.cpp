// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "SGameStateBase.h"
#include "SPlayerController.h"
#include "SPlayerState.h"

ASGameModeBase::ASGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ASGameStateBase::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
}
