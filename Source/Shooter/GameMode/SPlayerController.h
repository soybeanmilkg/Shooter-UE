// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOnPossessPlayerPawnDelegate);

/**
 * 
 */
UCLASS()
class SHOOTER_API ASPlayerController : public ACommonPlayerController
{
	GENERATED_BODY()

public:
	explicit ASPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	//~ Begin AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor

	//~ Begin AController
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;
	//~ End AController

public:
	UPROPERTY(BlueprintAssignable)
	FSOnPossessPlayerPawnDelegate OnPossessPlayerPawn {};
};
