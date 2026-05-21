// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

#include "SPlayerController.h"
#include "GameFramework/GameplayMessageSubsystem.h"

namespace Shooter::Tags
{
	UE_DEFINE_GAMEPLAY_TAG(PlayerStateReadyMessage, TEXT("GameplayMessage.PlayerStateReady"));
}

void ASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayer = Cast<ASPlayerController>(GetOwningController());
}

void ASPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASPlayerState::OnRep_PlayerId()
{
	Super::OnRep_PlayerId();

	BroadcastPlayerStateReady();
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
