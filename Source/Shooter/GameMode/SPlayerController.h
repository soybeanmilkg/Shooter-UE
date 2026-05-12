// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

class UInputMappingContext;

UENUM(BlueprintType)
enum class ESInputMode : uint8
{
	Game,
	UI,
	GameAndUI,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSOnPossessPlayerPawnDelegate);

/**
 * 
 */
UCLASS()
class SHOOTER_API ASPlayerController : public APlayerController
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

	UFUNCTION(BlueprintCallable, Category="Shooter|Input")
	void SetSInputMode(const ESInputMode NewInputMode);

	UFUNCTION(BlueprintPure, Category="Shooter|UI")
	UUserWidget* GetCurrentWidget() const { return CurrentWidget; }

	UFUNCTION(BlueprintCallable, Category="Shooter|UI")
	void Goto(const TSubclassOf<UUserWidget> ToWidgetClass);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooter")
	ESInputMode InputMode { ESInputMode::GameAndUI };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooter")
	TObjectPtr<UInputMappingContext> InputMappingContext { nullptr };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Shooter")
	TSubclassOf<UUserWidget> SetupWidget { nullptr };

	UPROPERTY(Transient)
	UUserWidget* CurrentWidget { nullptr };
};
