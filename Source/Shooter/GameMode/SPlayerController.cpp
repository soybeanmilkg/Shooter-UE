// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "EngineUtils.h"
#include "EnhancedInputSubsystems.h"
#include "Shooter.h"
#include "Blueprint/UserWidget.h"
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

		SetSInputMode(InputMode);

		if (SetupWidget)
		{
			CurrentWidget = CreateWidget<UUserWidget>(this, SetupWidget);
			CurrentWidget->AddToViewport();
		}

		if (InputMappingContext)
		{
			if (const ULocalPlayer* LocalPlayer = GetLocalPlayer())
			{
				if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
				{
					Subsystem->AddMappingContext(InputMappingContext, 0);
				}
			}
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

void ASPlayerController::SetSInputMode(const ESInputMode NewInputMode)
{
	if (!IsLocalController())
	{
		return;
	}

	InputMode = NewInputMode;
	switch (InputMode)
	{
		case ESInputMode::Game:
			SetInputMode(FInputModeGameOnly {});
			break;
		case ESInputMode::UI:
			SetInputMode(FInputModeUIOnly {});
			break;
		case ESInputMode::GameAndUI:
			SetInputMode(FInputModeGameAndUI {});
			break;
	}
}

void ASPlayerController::Goto(const TSubclassOf<UUserWidget> ToWidgetClass)
{
	if (ToWidgetClass == nullptr)
	{
		UE_LOG(LogShooter, Warning, TEXT("[ASPlayerController] widget class is nullptr."));
		return;
	}

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
	}

	CurrentWidget = CreateWidget<UUserWidget>(this, ToWidgetClass);
	CurrentWidget->AddToViewport();
}
