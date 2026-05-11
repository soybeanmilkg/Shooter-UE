// Fill out your copyright notice in the Description page of Project Settings.


#include "SBullet.h"

#include "PaperSpriteComponent.h"
#include "Character/SPawn.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ASBullet::ASBullet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	SetNetUpdateFrequency(100.f);
	// SetMinNetUpdateFrequency(60.f);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1200;
	ProjectileMovement->MaxSpeed = 1200;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void ASBullet::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	}
}

void ASBullet::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		Collision->OnComponentBeginOverlap.RemoveAll(this);
	}
}

void ASBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		const FVector Location = GetActorLocation();
		if (Location.X < -2000 || Location.X > 2000 || Location.Y < -1500 || Location.Y > 1500)
		{
			Destroy();
		}
	}
}

void ASBullet::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (TargetClass == nullptr) return;

	if (const auto AsPawn = Cast<ASPawn>(OtherActor))
	{
		if (AsPawn->IsAlive() && AsPawn->IsA(TargetClass))
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, UDamageType::StaticClass());
			Destroy();
		}
	}
}
