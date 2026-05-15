// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "GameExperienceManagerComponent.h"
#include "SGameStateBase.h"
#include "Shooter.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "SWorldSettings.h"

ASGameModeBase::ASGameModeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ASGameStateBase::StaticClass();
	PlayerControllerClass = ASPlayerController::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 等一帧，等一些初始化设置完成
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ASGameModeBase::HandleExperience);
}

void ASGameModeBase::HandleExperience() const
{
	FPrimaryAssetId ExperienceId;

	if (const auto WorldSettings = Cast<ASWorldSettings>(GetWorldSettings()))
	{
		ExperienceId = WorldSettings->GetGameExperienceAssetId();
	}

	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId("GameExperienceDefinition", "DA_DefaultExperience");
	}

	if (const auto AsGameState = GetGameState<ASGameStateBase>())
	{
		AsGameState->GetExperienceManager()->SetCurrentExperience(ExperienceId);
	}
	else
	{
		UE_LOG(LogShooter, Error, TEXT("[ASGameModeBase] GameState is nor a ASGameStateBase"));
	}
}
