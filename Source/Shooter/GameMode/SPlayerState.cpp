// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "Shooter.h"
#include "ShooterSaveData.h"
#include "SPlayerController.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"

namespace Shooter::Tags
{
	UE_DEFINE_GAMEPLAY_TAG(PlayerStateReadyMessage, TEXT("GameplayMessage.PlayerStateReady"));
}

void ASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayer = Cast<ASPlayerController>(GetOwningController());

	LoadSaveData();
}

void ASPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveData();
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerDisplayName)
}

void ASPlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();

	BroadcastPlayerStateReady();
}

FText ASPlayerState::GetPlayerName_Text() const
{
	if (PlayerSaveData)
	{
		return PlayerSaveData->PlayerName;
	}

	return PlayerDisplayName;
}

void ASPlayerState::SetPlayerName_Text(const FText InPlayerName)
{
	if (OwningPlayer && OwningPlayer->IsLocalController())
	{
		if (PlayerSaveData)
		{
			PlayerSaveData->PlayerName = InPlayerName;
			SaveData();
		}

		PlayerDisplayName = InPlayerName;
		SetPlayerDisplayName(InPlayerName);
	}
}

void ASPlayerState::BroadcastPlayerStateReady()
{
	if (const UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimerForNextTick([this]
		{
			FSPlayerStateReadyMessage Message;
			Message.PlayerState = this;
			UGameplayMessageSubsystem::Get(this).BroadcastMessage(Shooter::Tags::PlayerStateReadyMessage, Message);
		});
	}
}

void ASPlayerState::ChangeScore(const int32 Value)
{
	SetScore(GetScore() + Value);
}

void ASPlayerState::SetPlayerDisplayName_Implementation(const FText& InDisplayName)
{
	PlayerDisplayName = InDisplayName;
	OwningPlayer->ServerChangeName(InDisplayName.ToString());
}

void ASPlayerState::LoadSaveData()
{
	if (OwningPlayer && OwningPlayer->IsLocalController())
	{
		PlayerSaveData = Cast<USPlayerSaveData>(ULocalPlayerSaveGame::LoadOrCreateSaveGameForLocalPlayer(
			USPlayerSaveData::StaticClass(), OwningPlayer->GetLocalPlayer(), Shooter::SaveGame::Slot_Player));
		check(PlayerSaveData != nullptr);
		SetPlayerDisplayName(PlayerSaveData->PlayerName);
	}
}

void ASPlayerState::SaveData()
{
	if (PlayerSaveData != nullptr)
	{
		if (!PlayerSaveData->SaveGameToSlotForLocalPlayer())
		{
			UE_LOG(LogShooter, Warning, TEXT("[ASPlayerState] Failed to save player data"));
		}
	}
}
