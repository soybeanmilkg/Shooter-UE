// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SPawnMovementComponent.generated.h"

USTRUCT()
struct FSPawnMoveData
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 SerialId { 0 };

	UPROPERTY()
	float DeltaTime { 0.0f };

	UPROPERTY()
	FVector ControlInput {};

	UPROPERTY()
	FVector DesiredLocation {};
};

UCLASS(ClassGroup=(Movement), meta=(BlueprintSpawnableComponent))
class SHOOTER_API USPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	explicit USPawnMovementComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual float GetMaxSpeed() const override { return Speed; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooter")
	float Speed = { 800 };

	void ApplyControlInputToVelocity(FSPawnMoveData& MoveData);

	bool LimitWorldBounds();

	UPROPERTY(Transient)
	uint32 bPositionCorrected : 1;

	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;

private:
	uint32 MoveDataSerialId { 0 };

	bool MoveAutonomous(FSPawnMoveData& MoveData);

	UFUNCTION(Server, Reliable)
	void SendMoveDataToServer(const FSPawnMoveData& MoveData);

	UFUNCTION(NetMulticast, Reliable)
	void SyncLocation(const uint32 SerialId, const FVector_NetQuantize Location);

	bool IsClientControlled() const;
};
