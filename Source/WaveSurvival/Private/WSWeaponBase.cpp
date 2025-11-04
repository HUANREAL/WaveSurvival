// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSWeaponBase.h"
#include "WSPlayerState.h"
#include "WSEnemyBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

AWSWeaponBase::AWSWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create weapon mesh
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// Default weapon stats
	WeaponType = EWSWeaponType::AssaultPistol;
	FireMode = EWSWeaponFireMode::Hitscan;
	BaseDamage = 10.0f;
	FireRate = 0.1f; // 10 shots per second
	MagazineSize = 30;
	ReloadTime = 2.0f;
	Range = 5000.0f;
	bAutomatic = true;

	CurrentAmmo = MagazineSize;
	ElementalType = EWSElementalType::None;
	ElementalDamagePercent = 0.0f;

	bIsFiring = false;
	bIsReloading = false;
	TimeSinceLastShot = 0.0f;
}

void AWSWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmo = MagazineSize;
	
	UE_LOG(LogTemp, Log, TEXT("Weapon initialized: %d"), (int32)WeaponType);
}

void AWSWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastShot += DeltaTime;

	// Automatic fire
	if (bIsFiring && bAutomatic && CanFire())
	{
		if (TimeSinceLastShot >= FireRate)
		{
			Fire();
		}
	}
}

void AWSWeaponBase::StartFire()
{
	bIsFiring = true;

	if (CanFire())
	{
		Fire();
	}
	else if (NeedsReload())
	{
		Reload();
	}
}

void AWSWeaponBase::StopFire()
{
	bIsFiring = false;
}

void AWSWeaponBase::Fire()
{
	if (!CanFire())
	{
		return;
	}

	CurrentAmmo--;
	TimeSinceLastShot = 0.0f;

	// Perform attack based on fire mode
	switch (FireMode)
	{
		case EWSWeaponFireMode::Hitscan:
			PerformHitscan();
			break;
			
		case EWSWeaponFireMode::Projectile:
			PerformProjectile();
			break;
			
		case EWSWeaponFireMode::Melee:
			PerformMelee();
			break;
	}

	UE_LOG(LogTemp, Log, TEXT("Weapon fired - Ammo remaining: %d/%d"), CurrentAmmo, MagazineSize);

	// Auto-reload if empty
	if (CurrentAmmo == 0)
	{
		Reload();
	}
}

void AWSWeaponBase::Reload()
{
	if (bIsReloading || CurrentAmmo == MagazineSize)
	{
		return;
	}

	bIsReloading = true;

	// Get reload time with player stat modifiers
	float ActualReloadTime = ReloadTime;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AWSPlayerState* PS = Cast<AWSPlayerState>(OwnerCharacter->GetPlayerState());
		if (PS)
		{
			ActualReloadTime *= (1.0f / PS->PlayerStats.ReloadSpeed);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AWSWeaponBase::FinishReload, ActualReloadTime, false);
	
	UE_LOG(LogTemp, Log, TEXT("Reloading weapon - Time: %f seconds"), ActualReloadTime);
}

bool AWSWeaponBase::CanFire() const
{
	return !bIsReloading && CurrentAmmo > 0;
}

bool AWSWeaponBase::NeedsReload() const
{
	return CurrentAmmo == 0 && !bIsReloading;
}

void AWSWeaponBase::PerformHitscan()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	// Get camera location and direction
	FVector CameraLocation;
	FRotator CameraRotation;
	OwnerCharacter->GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector TraceStart = CameraLocation;
	FVector TraceEnd = TraceStart + (CameraRotation.Vector() * Range);

	// Perform line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		QueryParams
	);

	if (bHit)
	{
		// Check if we hit an enemy
		AWSEnemyBase* Enemy = Cast<AWSEnemyBase>(HitResult.GetActor());
		if (Enemy)
		{
			bool bIsCritical = false;
			float Damage = CalculateDamage(bIsCritical);
			
			Enemy->TakeDamageCustom(Damage, OwnerCharacter, bIsCritical);
			
			// Apply elemental effect
			if (ElementalType != EWSElementalType::None)
			{
				ApplyElementalEffect(Enemy);
			}

			// Award kill credit
			AWSPlayerState* PS = Cast<AWSPlayerState>(OwnerCharacter->GetPlayerState());
			if (PS && Enemy->EnemyStats.CurrentHealth <= 0)
			{
				PS->OnKill();
			}
		}

		// Debug draw
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 1.0f, 0, 1.0f);
	}
	else
	{
		// Debug draw
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
	}
}

void AWSWeaponBase::PerformProjectile()
{
	// Projectile implementation would spawn a projectile actor
	UE_LOG(LogTemp, Log, TEXT("Projectile fired"));
}

void AWSWeaponBase::PerformMelee()
{
	// Melee implementation would do a sphere trace
	UE_LOG(LogTemp, Log, TEXT("Melee attack"));
}

void AWSWeaponBase::FinishReload()
{
	bIsReloading = false;
	CurrentAmmo = MagazineSize;
	
	UE_LOG(LogTemp, Log, TEXT("Reload complete"));
}

float AWSWeaponBase::CalculateDamage(bool& bOutIsCritical)
{
	float FinalDamage = BaseDamage;
	bOutIsCritical = false;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AWSPlayerState* PS = Cast<AWSPlayerState>(OwnerCharacter->GetPlayerState());
		if (PS)
		{
			// Apply damage multiplier
			FinalDamage *= PS->PlayerStats.DamageMultiplier;

			// Check for critical hit
			float CritRoll = FMath::FRand();
			if (CritRoll < PS->PlayerStats.CriticalChance)
			{
				bOutIsCritical = true;
				FinalDamage *= PS->PlayerStats.CriticalDamageMultiplier;
			}
		}
	}

	// Add elemental damage
	if (ElementalType != EWSElementalType::None)
	{
		FinalDamage += (FinalDamage * ElementalDamagePercent);
	}

	return FinalDamage;
}

void AWSWeaponBase::ApplyElementalEffect(AActor* Target)
{
	// Apply elemental status effects based on type
	switch (ElementalType)
	{
		case EWSElementalType::Fire:
			// Apply burn damage over time
			UE_LOG(LogTemp, Log, TEXT("Applied fire effect"));
			break;
			
		case EWSElementalType::Ice:
			// Slow enemy
			UE_LOG(LogTemp, Log, TEXT("Applied ice effect"));
			break;
			
		case EWSElementalType::Electric:
			// Chain damage to nearby enemies
			UE_LOG(LogTemp, Log, TEXT("Applied electric effect"));
			break;
			
		case EWSElementalType::Acid:
			// Reduce armor
			UE_LOG(LogTemp, Log, TEXT("Applied acid effect"));
			break;
			
		case EWSElementalType::Poison:
			// Poison damage over time
			UE_LOG(LogTemp, Log, TEXT("Applied poison effect"));
			break;
			
		default:
			break;
	}
}
