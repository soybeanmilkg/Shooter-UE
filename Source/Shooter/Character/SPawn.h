// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPawn.h"
#include "NativeGameplayTags.h"
#include "Engine/DamageEvents.h"
#include "SPawn.generated.h"

class USPawnMovementComponent;
class UPaperFlipbookComponent;
class UPaperSpriteComponent;
class UBoxComponent;
class ASBullet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedDelegate, float, Health, float, OldHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeadDelegate);

USTRUCT(BlueprintType)
struct FSPawnDieMessage
{
	GENERATED_BODY()

	FDamageEvent DamageEvent {};

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AController> Instigator { nullptr };

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Causer { nullptr };

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ASPawn> DeathPawn { nullptr };
};

namespace Shooter::Tags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PawnDieMessage);
}

UCLASS(Abstract)
class SHOOTER_API ASPawn : public AModularPawn
{
	GENERATED_BODY()

public:
	explicit ASPawn(const FObjectInitializer& ObjectInitializer);

protected:
	//~ Begin AActor
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//~ End AActor

	//~ Begin UObject
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//~ End UObject
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealthChangedDelegate OnHealthChanged {};

	UPROPERTY(BlueprintAssignable)
	FOnDeadDelegate OnDead {};

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ChangeHealth(const float Value);

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnHit();

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnDie();

	UFUNCTION(BlueprintPure)
	bool IsAlive() const { return !bIsDead; }

	UFUNCTION(Server, Reliable, BlueprintCallable)
	virtual void ShootBullet(FVector_NetQuantize Direction);

	UFUNCTION(NetMulticast, Reliable)
	virtual void OnShoot();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter")
	float ShootTime { 1.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter")
	TSubclassOf<ASBullet> BulletClass { nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter")
	TSubclassOf<ASPawn> EnemyClass { nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter|音效")
	TObjectPtr<USoundBase> ShootSound { nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter|音效")
	TObjectPtr<USoundBase> HitSound { nullptr };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter|音效")
	TObjectPtr<USoundBase> DestroySound { nullptr };

	UPROPERTY(BlueprintReadOnly, Category="Shooter|属性", ReplicatedUsing=OnRep_Health)
	float Health { 0 };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Shooter|属性", Replicated)
	float MaxHealth { 3 };

	UFUNCTION()
	void OnRep_Health(const float OldHealth);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Collision { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperSpriteComponent> Sprite { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> ShootPoint { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UPaperFlipbookComponent> DestroyEffect { nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USPawnMovementComponent> PawnMovement { nullptr };

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsDead { false };
	float ShootTimer { 0.f };
};
