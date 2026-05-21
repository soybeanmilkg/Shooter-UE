// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameStateBase.h"

#include "GameExperienceManagerComponent.h"
#include "Shooter.h"
#include "SPlayerController.h"
#include "SPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

struct FSScoreInfoComparer
{
	bool operator()(const FSPlayerScoreInfo& A, const FSPlayerScoreInfo& B) const
	{
		if (FMath::IsNearlyEqual(A.Score, B.Score))
		{
			// 分数相同时，按名字字母顺序升序排列
			return A.PlayerName < B.PlayerName;
		}

		return A.Score < B.Score;
	}
};

ASGameStateBase::ASGameStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManager = CreateDefaultSubobject<UGameExperienceManagerComponent>(TEXT("ExperienceManager"));
}

void ASGameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if (!UKismetSystemLibrary::IsDedicatedServer(this) && BackgroundMusic != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, BackgroundMusic);
	}

	LoadSaveData();
}

void ASGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveData();
}

void ASGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ScoreInfos)
	DOREPLIFETIME(ThisClass, PlayerIds)
}

TArray<FSPlayerScoreInfo> ASGameStateBase::GetScoreInfos() const
{
	if (ScoreInfos.IsEmpty())
	{
		return {};
	}

	TArray<FSPlayerScoreInfo> Heap = ScoreInfos;
	TArray<FSPlayerScoreInfo> Output {};

	Output.SetNum(Heap.Num());

	constexpr FSScoreInfoComparer Comparer {};
	while (Heap.Num() > 0)
	{
		FSPlayerScoreInfo Info {};
		Heap.HeapPop(Info, Comparer);
		Output[Heap.Num()] = Info;
	}

	return Output;
}

void ASGameStateBase::RecordScoreInfo(ASPlayerController* PlayerController)
{
	if (PlayerController == nullptr)
	{
		return;
	}

	if (const auto PlayerState = PlayerController->GetPlayerState<ASPlayerState>())
	{
		const FSPlayerScoreInfo NewScoreInfo { PlayerState->GetPlayerName(), PlayerState->GetScore() };
		constexpr FSScoreInfoComparer Comparer {};
		ScoreInfos.HeapPush(NewScoreInfo, Comparer);

		// 删除多余的分数信息，堆顶为排序最后的分数信息
		FSPlayerScoreInfo TempInfo {};
		while (ScoreInfos.Num() > 10)
		{
			ScoreInfos.HeapPop(TempInfo, Comparer);
		}

		SaveData();
	}
}

void ASGameStateBase::AddPlayerId(const int32 PlayerId)
{
	PlayerIds.AddUnique(PlayerId);
	OnPlayerIdsChanged.Broadcast();
}

void ASGameStateBase::RemovePlayerId(const int32 PlayerId)
{
	PlayerIds.Remove(PlayerId);
	OnPlayerIdsChanged.Broadcast();
}

APlayerState* ASGameStateBase::GetPlayerStateByPlayerId(UObject* WorldContextObject, const int32 PlayerId)
{
	if (AGameStateBase* GameState = UGameplayStatics::GetGameState(WorldContextObject))
	{
		for (auto& PlayerState : GameState->PlayerArray)
		{
			if (PlayerState->GetPlayerId() == PlayerId)
			{
				return PlayerState;
			}
		}
	}

	return nullptr;
}

void ASGameStateBase::OnRep_PlayerIds()
{
	OnPlayerIdsChanged.Broadcast();
}

void ASGameStateBase::LoadSaveData()
{
	if (HasAuthority())
	{
		ServerSaveData = Cast<USServerSaveData>(UGameplayStatics::LoadGameFromSlot(Shooter::SaveGame::Slot_Server, 0));
		if (ServerSaveData == nullptr)
		{
			ServerSaveData = NewObject<USServerSaveData>();
			SaveData();
		}
		ScoreInfos = ServerSaveData->ScoreInfos;
	}
}

void ASGameStateBase::SaveData()
{
	if (ServerSaveData != nullptr)
	{
		ServerSaveData->ScoreInfos = ScoreInfos;
		if (!UGameplayStatics::SaveGameToSlot(ServerSaveData, Shooter::SaveGame::Slot_Server, 0))
		{
			UE_LOG(LogShooter, Warning, TEXT("[ASGameStateBase] Failed to save server data"));
		}
	}
}
