// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSCharacterBase.h"
#include "WSPlayerState.h"
#include "WSWeaponBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AWSCharacterBase::AWSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Character movement
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// Character class
	CharacterClass = EWSCharacterClass::Rogue;

	// Ability cooldowns (default values)
	Ability1Cooldown = 10.0f;
	Ability2Cooldown = 15.0f;
	UltimateCooldown = 60.0f;

	Ability1RemainingCooldown = 0.0f;
	Ability2RemainingCooldown = 0.0f;
	UltimateRemainingCooldown = 0.0f;

	bIsFiring = false;
	bIsReloading = false;
	bIsDowned = false;
}

void AWSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Get player state
	WSPlayerState = Cast<AWSPlayerState>(GetPlayerState());
	if (WSPlayerState)
	{
		WSPlayerState->CharacterClass = CharacterClass;
	}

	// Spawn weapon
	if (WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		CurrentWeapon = GetWorld()->SpawnActor<AWSWeaponBase>(WeaponClass, SpawnParams);
		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Character spawned: Class %d"), (int32)CharacterClass);
}

void AWSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCooldowns(DeltaTime);
	ApplyHealthRegen(DeltaTime);
}

void AWSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AWSCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWSCharacterBase::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AWSCharacterBase::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AWSCharacterBase::LookUp);

	// Jumping
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AWSCharacterBase::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AWSCharacterBase::StopJump);

	// Combat
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AWSCharacterBase::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AWSCharacterBase::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AWSCharacterBase::Reload);

	// Abilities
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &AWSCharacterBase::UseAbility1);
	PlayerInputComponent->BindAction("Ability2", IE_Pressed, this, &AWSCharacterBase::UseAbility2);
	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &AWSCharacterBase::UseUltimate);
}

void AWSCharacterBase::Fire()
{
	if (bIsDowned || bIsReloading || !CurrentWeapon)
	{
		return;
	}

	bIsFiring = true;
	CurrentWeapon->StartFire();
	
	UE_LOG(LogTemp, Log, TEXT("Firing weapon"));
}

void AWSCharacterBase::StopFire()
{
	bIsFiring = false;
	
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void AWSCharacterBase::Reload()
{
	if (bIsDowned || bIsReloading || !CurrentWeapon)
	{
		return;
	}

	CurrentWeapon->Reload();
	
	UE_LOG(LogTemp, Log, TEXT("Reloading weapon"));
}

void AWSCharacterBase::TakeDamageCustom(float DamageAmount, AActor* DamageCauser)
{
	if (!WSPlayerState || bIsDowned)
	{
		return;
	}

	// Apply damage reduction
	float FinalDamage = DamageAmount * (1.0f - WSPlayerState->PlayerStats.DamageReduction);
	
	WSPlayerState->PlayerStats.CurrentHealth -= FinalDamage;

	if (WSPlayerState->PlayerStats.CurrentHealth <= 0)
	{
		// Player downed
		bIsDowned = true;
		WSPlayerState->CurrentState = EWSPlayerState::Downed;
		WSPlayerState->OnDeath();
		
		UE_LOG(LogTemp, Log, TEXT("Player downed"));
	}
	
	UE_LOG(LogTemp, Log, TEXT("Player took %f damage, Health: %f"), 
		FinalDamage, WSPlayerState->PlayerStats.CurrentHealth);
}

void AWSCharacterBase::UseAbility1()
{
	if (bIsDowned || Ability1RemainingCooldown > 0.0f)
	{
		return;
	}

	// Apply cooldown reduction
	float ActualCooldown = Ability1Cooldown;
	if (WSPlayerState)
	{
		ActualCooldown *= (1.0f - WSPlayerState->PlayerStats.CooldownReduction);
	}

	Ability1RemainingCooldown = ActualCooldown;
	
	UE_LOG(LogTemp, Log, TEXT("Used Ability 1"));
	
	// Specific ability implementation in derived classes
}

void AWSCharacterBase::UseAbility2()
{
	if (bIsDowned || Ability2RemainingCooldown > 0.0f)
	{
		return;
	}

	// Apply cooldown reduction
	float ActualCooldown = Ability2Cooldown;
	if (WSPlayerState)
	{
		ActualCooldown *= (1.0f - WSPlayerState->PlayerStats.CooldownReduction);
	}

	Ability2RemainingCooldown = ActualCooldown;
	
	UE_LOG(LogTemp, Log, TEXT("Used Ability 2"));
	
	// Specific ability implementation in derived classes
}

void AWSCharacterBase::UseUltimate()
{
	if (bIsDowned || UltimateRemainingCooldown > 0.0f)
	{
		return;
	}

	// Apply cooldown reduction
	float ActualCooldown = UltimateCooldown;
	if (WSPlayerState)
	{
		ActualCooldown *= (1.0f - WSPlayerState->PlayerStats.CooldownReduction);
	}

	UltimateRemainingCooldown = ActualCooldown;
	
	UE_LOG(LogTemp, Log, TEXT("Used Ultimate"));
	
	// Specific ability implementation in derived classes
}

void AWSCharacterBase::Revive()
{
	if (!bIsDowned)
	{
		return;
	}

	bIsDowned = false;
	
	if (WSPlayerState)
	{
		WSPlayerState->OnRevive();
	}
	
	UE_LOG(LogTemp, Log, TEXT("Player revived"));
}

bool AWSCharacterBase::IsDowned() const
{
	return bIsDowned;
}

void AWSCharacterBase::MoveForward(float Value)
{
	if (bIsDowned || Value == 0.0f)
	{
		return;
	}

	AddMovementInput(GetActorForwardVector(), Value);
}

void AWSCharacterBase::MoveRight(float Value)
{
	if (bIsDowned || Value == 0.0f)
	{
		return;
	}

	AddMovementInput(GetActorRightVector(), Value);
}

void AWSCharacterBase::Turn(float Value)
{
	if (bIsDowned)
	{
		return;
	}

	AddControllerYawInput(Value);
}

void AWSCharacterBase::LookUp(float Value)
{
	if (bIsDowned)
	{
		return;
	}

	AddControllerPitchInput(Value);
}

void AWSCharacterBase::StartJump()
{
	if (!bIsDowned)
	{
		Jump();
	}
}

void AWSCharacterBase::StopJump()
{
	StopJumping();
}

void AWSCharacterBase::UpdateCooldowns(float DeltaTime)
{
	if (Ability1RemainingCooldown > 0.0f)
	{
		Ability1RemainingCooldown -= DeltaTime;
	}

	if (Ability2RemainingCooldown > 0.0f)
	{
		Ability2RemainingCooldown -= DeltaTime;
	}

	if (UltimateRemainingCooldown > 0.0f)
	{
		UltimateRemainingCooldown -= DeltaTime;
	}
}

void AWSCharacterBase::ApplyHealthRegen(float DeltaTime)
{
	if (!WSPlayerState || bIsDowned)
	{
		return;
	}

	if (WSPlayerState->PlayerStats.HealthRegenRate > 0.0f)
	{
		float RegenAmount = WSPlayerState->PlayerStats.HealthRegenRate * DeltaTime;
		WSPlayerState->PlayerStats.CurrentHealth = FMath::Min(
			WSPlayerState->PlayerStats.CurrentHealth + RegenAmount,
			WSPlayerState->PlayerStats.MaxHealth
		);
	}
}
