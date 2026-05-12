// Fill out your copyright notice in the Description page of Project Settings.


#include "SPawn.h"

#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Bullet/SBullet.h"
#include "Shooter.h"
#include "SPawnMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ASPawn::ASPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(true);
	SetNetUpdateFrequency(100.f);
	// SetMinNetUpdateFrequency(60.f);

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->CanCharacterStepUpOn = ECB_No;
	Collision->SetCollisionProfileName(TEXT("Pawn"));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetCollisionProfileName(TEXT("CharacterMesh"));

	ShootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ShootPoint"));
	ShootPoint->SetupAttachment(RootComponent);

	DestroyEffect = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DestroyEffect"));
	DestroyEffect->SetupAttachment(RootComponent);
	DestroyEffect->SetVisibility(false);
	DestroyEffect->SetLooping(false);

	PawnMovement = CreateDefaultSubobject<USPawnMovementComponent>(TEXT("PawnMovement"));
}

void ASPawn::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if (HasAuthority())
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	}
}

void ASPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsAlive() && ShootTimer > 0)
	{
		ShootTimer -= DeltaSeconds;
	}
}

void ASPawn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		Collision->OnComponentBeginOverlap.RemoveDynamic(this, &ThisClass::OnBeginOverlap);
	}
}

float ASPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsAlive())
	{
		return 0;
	}

	OnHit();
	ChangeHealth(-DamageAmount);

	if (Health < 0.1f)
	{
		OnDie();

		if (EventInstigator != nullptr)
		{
			if (const auto State = EventInstigator->GetPlayerState<APlayerState>())
			{
				State->SetScore(State->GetScore() + 1);
			}
		}
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Health);
	DOREPLIFETIME(ThisClass, MaxHealth);
}

void ASPawn::OnHit_Implementation()
{
	if (HitSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, HitSound);
	}
}

void ASPawn::ShootBullet_Implementation(FVector_NetQuantize Direction)
{
	if (!IsAlive() || ShootTimer > 0) return;

	ShootTimer = ShootTime;

	UWorld* World = GetWorld();
	check(World != nullptr);

	FActorSpawnParameters SpawnInfo {};
	SpawnInfo.Instigator = this;
	ASBullet* Bullet = World->SpawnActor<ASBullet>(BulletClass, ShootPoint->GetComponentTransform(), SpawnInfo);
	if (Bullet == nullptr)
	{
		UE_LOG(LogShooter, Warning, TEXT("[ASPawn] Spawn Bullet %s Failed."), *GetNameSafe(BulletClass))
		return;
	}

	Bullet->TargetClass = EnemyClass;

	if (!Direction.IsNearlyZero())
	{
		Direction.Normalize();
		Direction.Z = 0;
		UProjectileMovementComponent* ProjectileMovement = Bullet->GetProjectileMovementComponent();
		ProjectileMovement->Velocity = Direction * ProjectileMovement->GetMaxSpeed();
	}

	OnShoot();
}

void ASPawn::OnShoot_Implementation()
{
	if (ShootSound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, ShootSound);
	}
}

void ASPawn::OnDie_Implementation()
{
	if (bIsDead) return;

	UE_LOG(LogShooter, Log, TEXT("[ASPawn][%s][%s] %s On Die"), *Shooter::GetNetModeString(this), *Shooter::GetNetRoleString(this), *GetNameSafe(this))

	bIsDead = true;

	OnDead.Broadcast();

	SetLifeSpan(2.f);

	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sprite->SetVisibility(false);

	DestroyEffect->SetVisibility(true);
	DestroyEffect->PlayFromStart();

	if (DestroySound != nullptr)
	{
		UGameplayStatics::PlaySound2D(this, DestroySound);
	}
}

void ASPawn::OnRep_Health(const float OldHealth)
{
	UE_LOG(LogShooter, Log, TEXT("[ASPawn] OnRep Health, OldHealth = %.2f, Health = %.2f"), OldHealth, Health);
	OnHealthChanged.Broadcast(Health, OldHealth);
}

void ASPawn::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASPawn* AsPawn = Cast<ASPawn>(OtherActor);
	if (AsPawn != nullptr && AsPawn->IsA(EnemyClass) && IsAlive() && AsPawn->IsAlive())
	{
		OnDie();
		AsPawn->OnDie();
	}
}

void ASPawn::ChangeHealth_Implementation(const float Value)
{
	const float NewHealth = FMath::Clamp(Health + Value, 0, MaxHealth);
	if (FMath::IsNearlyEqual(NewHealth, Health))
	{
		return;
	}

	const float OldHealth = Health;
	Health = NewHealth;
	OnHealthChanged.Broadcast(Health, OldHealth);
}
