// Fill out your copyright notice in the Description page of Project Settings.


#include "SEnemy.h"

#include "SPlayer.h"
#include "Pickup/SPickup.h"

ASEnemy::ASEnemy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	ShootTime = 3.f;
	EnemyClass = ASPlayer::StaticClass();
}

void ASEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		if (IsAlive())
		{
			if (bAutoMove)
			{
				AddMovementInput(FVector(-1, 0, 0), 1);
			}

			if (bAutoShoot)
			{
				ShootBullet(ShootDirection);
			}
		}

		const FVector Location = GetActorLocation();
		if (Location.X < -2000 || Location.X > 2500 || Location.Y < -1500 || Location.Y > 1500)
		{
			Destroy();
		}
	}
}

void ASEnemy::OnDie_Implementation()
{
	Super::OnDie_Implementation();

	if (HasAuthority())
	{
		if (DropPickups.Num() > 0 && FMath::RandBool())
		{
			const TSubclassOf<ASPickup>& DropPickup = DropPickups[FMath::RandHelper(DropPickups.Num())];
			GetWorld()->SpawnActor<ASPickup>(DropPickup, GetActorTransform());
		}
	}
}
