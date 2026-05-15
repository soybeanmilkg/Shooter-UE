// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "EngineUtils.h"
#include "Camera/CameraActor.h"

ASPlayerController::ASPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (IsLocalController() && World)
	{
		if (const TActorIterator<ACameraActor> It(World); It)
		{
			SetViewTarget(*It);
		}
	}
}

void ASPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ASPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	OnPossessPlayerPawn.Broadcast();
}

void ASPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	OnPossessPlayerPawn.Broadcast();
}
