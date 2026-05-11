// Fill out your copyright notice in the Description page of Project Settings.


#include "SPawnMovementComponent.h"

#include "Shooter.h"

USPawnMovementComponent::USPawnMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USPawnMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USPawnMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PawnOwner == nullptr || UpdatedComponent == nullptr)
	{
		return;
	}

	FSPawnMoveData MoveData;
	MoveData.DeltaTime = DeltaTime;
	if (MoveAutonomous(MoveData) && IsClientControlled())
	{
		// 将客户端移动数据发送到服务器
		SendMoveDataToServer(MoveData);
	}
}

void USPawnMovementComponent::ApplyControlInputToVelocity(FSPawnMoveData& MoveData)
{
	FVector ControlInput = MoveData.ControlInput;
	// 对于非玩家控制的 Pawn，简单的移动逻辑，没有 Controller，也需要读取 InputVector
	if (PawnOwner && (!PawnOwner->GetController() || PawnOwner->IsLocallyControlled()))
	{
		ControlInput = GetPendingInputVector();
		MoveData.ControlInput = ControlInput;
	}

	if (ControlInput.SizeSquared() > 0.f)
	{
		// 输入为移动方向，朝向改方向，以最大速度移动
		Velocity = ControlInput.GetClampedToMaxSize(1.f) * GetMaxSpeed();
	}
	else
	{
		// 没有输入，速度直接为 0
		Velocity = FVector::ZeroVector;
	}

	ConsumeInputVector();
}

bool USPawnMovementComponent::LimitWorldBounds()
{
	const AWorldSettings* WorldSettings = PawnOwner ? PawnOwner->GetWorldSettings() : nullptr;
	if (!WorldSettings || !WorldSettings->AreWorldBoundsChecksEnabled() || !UpdatedComponent)
	{
		return false;
	}

	const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	if (CurrentLocation.Z < WorldSettings->KillZ)
	{
		Velocity.Z = FMath::Min<FVector::FReal>(GetMaxSpeed(), WorldSettings->KillZ - CurrentLocation.Z + 2.0f);
		return true;
	}

	return false;
}

bool USPawnMovementComponent::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation)
{
	bPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotation);
	return bPositionCorrected;
}

bool USPawnMovementComponent::MoveAutonomous(FSPawnMoveData& MoveData)
{
	ApplyControlInputToVelocity(MoveData);
	LimitWorldBounds();

	bPositionCorrected = false;
	bool Moved = false;

	const FVector Delta = Velocity * MoveData.DeltaTime;

	if (!Delta.IsNearlyZero(1e-6f))
	{
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FQuat Rotation = UpdatedComponent->GetComponentQuat();

		FHitResult Hit(1.f);
		SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

		if (Hit.IsValidBlockingHit())
		{
			HandleImpact(Hit, MoveData.DeltaTime, Delta);
			// Try to slide the remaining distance along the surface.
			SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
		}

		// Update velocity
		// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
		if (!bPositionCorrected)
		{
			const FVector NewLocation = UpdatedComponent->GetComponentLocation();
			Velocity = (NewLocation - OldLocation) / MoveData.DeltaTime;
		}

		Moved = true;

		if (IsClientControlled())
		{
			MoveData.SerialId = ++MoveDataSerialId;
			MoveData.DesiredLocation = UpdatedComponent->GetComponentLocation();
		}

		UE_LOG(LogShooter, VeryVerbose, TEXT("[USPawnMovementComponent][%s][%s] Move State: SerialId = %d, Velocity = %s, NewLocation = %s, OldLocation = %s"),
		       *Shooter::GetNetModeString(this), *Shooter::GetNetRoleString(this),
		       MoveData.SerialId, *Velocity.ToString(), *UpdatedComponent->GetComponentLocation().ToString(), *OldLocation.ToString()
		);
	}

	// Finalize
	UpdateComponentVelocity();

	return Moved;
}

bool USPawnMovementComponent::IsClientControlled() const
{
	return GetNetMode() == NM_Client && GetOwnerRole() == ROLE_AutonomousProxy;
}

void USPawnMovementComponent::SendMoveDataToServer_Implementation(const FSPawnMoveData& MoveData)
{
	if (UpdatedComponent == nullptr)
	{
		return;
	}

	FSPawnMoveData NewMoveData = MoveData;
	const bool bMoveResult = MoveAutonomous(NewMoveData);

	const FVector NewLocation = UpdatedComponent->GetComponentLocation();
	UE_LOG(LogShooter, VeryVerbose, TEXT("[USPawnMovementComponent][%s][%s] MoveResult = %s, NewLocation = %s, DesiredLocation = %s"),
	       *Shooter::GetNetModeString(this), *Shooter::GetNetRoleString(this),
	       bMoveResult ? TEXT("True") : TEXT("False"),
	       *NewLocation.ToString(), *MoveData.DesiredLocation.ToString()
	);

	if (!bMoveResult || !NewLocation.Equals(MoveData.DesiredLocation, 1.f))
	{
		UE_LOG(LogShooter, Warning, TEXT("[USPawnMovementComponent] Server Move Failed"));
		// 服务器移动执行失败，或者是服务器执行结果和客户端不符合，通知客户端回滚
		SyncLocation(MoveData.SerialId, NewLocation);
	}
}

void USPawnMovementComponent::SyncLocation_Implementation(const uint32 SerialId, const FVector_NetQuantize Location)
{
	if (IsClientControlled())
	{
		// 重置 SerialId 到出错的上一位，下次移动时，重新发送改 SerialId
		MoveDataSerialId = SerialId > 0 ? SerialId - 1 : 0;
	}

	if (UpdatedComponent != nullptr)
	{
		UpdatedComponent->SetWorldLocation(Location);
	}
}
