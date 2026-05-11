// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickup.generated.h"

class ASPlayer;
class UPaperSpriteComponent;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class SHOOTER_API ASPickup : public AActor
{
	GENERATED_BODY()

public:
	explicit ASPickup(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	//~ Begin AActor
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor

public:
	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ApplyEffect(ASPlayer* Player);

	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Apply Effect", BlueprintAuthorityOnly)
	void ReceiveOnApplyEffect(ASPlayer* Player);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Collision { nullptr };

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperSpriteComponent> Sprite { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement { nullptr };
};
