// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "SBullet.generated.h"

class ASPawn;
class UPaperSpriteComponent;
class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class SHOOTER_API ASBullet : public AActor
{
	GENERATED_BODY()

public:
	explicit ASBullet(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter")
	TSubclassOf<ASPawn> TargetClass { nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter")
	float Damage { 1.f };

	UFUNCTION(BlueprintPure)
	UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Collision { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperSpriteComponent> Sprite { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement { nullptr };

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
