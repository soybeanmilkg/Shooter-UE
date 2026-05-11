// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayer.h"

#include "SEnemy.h"
#include "Kismet/GameplayStatics.h"

ASPlayer::ASPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyClass = ASEnemy::StaticClass();
}

void ASPlayer::Move(const float X, const float Y)
{
	if (IsAlive())
	{
		AddMovementInput(FVector(X, Y, 0.f), 1);
	}
}

void ASPlayer::OnPickup_Implementation()
{
	if (PickupSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, PickupSound);
	}
}
