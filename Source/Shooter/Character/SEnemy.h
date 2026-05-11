// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPawn.h"
#include "SEnemy.generated.h"

class ASPickup;

UCLASS()
class SHOOTER_API ASEnemy : public ASPawn
{
	GENERATED_BODY()

public:
	explicit ASEnemy(const FObjectInitializer& ObjectInitializer);

protected:
	//~ Begin AActor
	virtual void Tick(float DeltaSeconds) override;
	//~ End AActor

	//~ Begin ASPawn
	virtual void OnDie_Implementation() override;
	//~ End ASPawn

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	bool bAutoMove { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	bool bAutoShoot { true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	FVector ShootDirection { -1, 0, 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	TArray<TSubclassOf<ASPickup>> DropPickups {};
};
