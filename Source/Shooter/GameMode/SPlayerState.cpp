// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "Shooter.h"
#include "ShooterSaveData.h"
#include "SPlayerController.h"
#include "Kismet/GameplayStatics.h"

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

FText ASPlayerState::GetPlayerName_Text() const
{
	if (PlayerSaveData)
	{
		return PlayerSaveData->PlayerName;
	}

	return FText::GetEmpty();
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

		OwningPlayer->ServerChangeName(PlayerSaveData->PlayerName.ToString());
	}
}

void ASPlayerState::LoadSaveData()
{
	if (OwningPlayer && OwningPlayer->IsLocalController())
	{
		PlayerSaveData = Cast<USPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(Shooter::SaveGame::Slot_Player, 0));
		if (PlayerSaveData == nullptr)
		{
			PlayerSaveData = NewObject<USPlayerSaveData>();
			SaveData();
		}

		OwningPlayer->ServerChangeName(PlayerSaveData->PlayerName.ToString());
	}
}

void ASPlayerState::SaveData()
{
	if (PlayerSaveData != nullptr)
	{
		if (!UGameplayStatics::SaveGameToSlot(PlayerSaveData, Shooter::SaveGame::Slot_Player, 0))
		{
			UE_LOG(LogShooter, Warning, TEXT("[ASPlayerState] Failed to save player data"));
		}
	}
}
