// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPawn.h"
#include "SPlayer.generated.h"

UCLASS()
class SHOOTER_API ASPlayer : public ASPawn
{
	GENERATED_BODY()

public:
	explicit ASPlayer(const FObjectInitializer& ObjectInitializer);

public:
	UFUNCTION(BlueprintCallable)
	void Move(const float X, const float Y);

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnPickup();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter|音效")
	TObjectPtr<USoundBase> PickupSound { nullptr };
};
