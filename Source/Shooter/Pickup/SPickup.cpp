// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickup.h"

#include "PaperSpriteComponent.h"
#include "Character/SPlayer.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASPickup::ASPickup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 0;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void ASPickup::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);

		ProjectileMovement->MaxSpeed = FMath::RandRange(400, 600);
		const FVector Direction = UKismetMathLibrary::RotateAngleAxis(FVector::ForwardVector, FMath::RandRange(0, 360), FVector::UpVector);
		ProjectileMovement->Velocity = Direction * ProjectileMovement->MaxSpeed;

		SetLifeSpan(FMath::RandRange(10, 15));
	}
}

void ASPickup::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		Collision->OnComponentBeginOverlap.RemoveAll(this);
	}
}

void ASPickup::ApplyEffect_Implementation(ASPlayer* Player)
{
	ReceiveOnApplyEffect(Player);
	Player->OnPickup();
}

void ASPickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto PlayerPawn = Cast<ASPlayer>(OtherActor))
	{
		if (PlayerPawn->IsAlive())
		{
			ApplyEffect(PlayerPawn);
			Destroy();
		}
	}
	else if (OtherActor->ActorHasTag(FName("Border")))
	{
		ProjectileMovement->Velocity = UKismetMathLibrary::GetReflectionVector(ProjectileMovement->Velocity, SweepResult.Normal);
	}
}
