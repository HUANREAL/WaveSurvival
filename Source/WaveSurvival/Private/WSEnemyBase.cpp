// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSEnemyBase.h"
#include "WSGameState.h"
#include "WSPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

AWSEnemyBase::AWSEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bIsBoss = false;
	CurrentTarget = nullptr;
}

void AWSEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	// Initialize health
	EnemyStats.CurrentHealth = EnemyStats.MaxHealth;
	
	UE_LOG(LogTemp, Log, TEXT("Enemy spawned: %d with %f health"), 
		(int32)EnemyType, EnemyStats.MaxHealth);
}

void AWSEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update target
	if (bIsBoss)
	{
		// Bosses target highest damage dealer
		CurrentTarget = FindHighestDamageDealer();
	}
	else
	{
		// Regular enemies target nearest player
		CurrentTarget = FindNearestPlayer();
	}

	// Move towards target (AI would handle this in a real implementation)
}

bool AWSEnemyBase::TakeDamageCustom(float DamageAmount, AActor* DamageCauser, bool bIsCritical)
{
	if (EnemyStats.CurrentHealth <= 0)
	{
		return false;
	}

	// Check if can take critical damage
	if (bIsCritical && !EnemyStats.bCanCrit)
	{
		bIsCritical = false;
	}

	// Apply armor reduction with clamped armor value
	float ClampedArmor = FMath::Clamp(EnemyStats.Armor, 0.0f, 1.0f);
	float FinalDamage = DamageAmount * (1.0f - ClampedArmor);

	// Apply critical multiplier
	if (bIsCritical)
	{
		FinalDamage *= CriticalDamageMultiplier;
	}

	EnemyStats.CurrentHealth -= FinalDamage;

	// Track damage from players
	if (DamageCauser)
	{
		if (DamageReceivedFromPlayers.Contains(DamageCauser))
		{
			DamageReceivedFromPlayers[DamageCauser] += FinalDamage;
		}
		else
		{
			DamageReceivedFromPlayers.Add(DamageCauser, FinalDamage);
		}
	}

	UpdateHealthBar();

	bool bKilledEnemy = false;
	if (EnemyStats.CurrentHealth <= 0)
	{
		bKilledEnemy = true;
		Die();
	}
	
	UE_LOG(LogTemp, Log, TEXT("Enemy took %f damage (Critical: %d), Health: %f"), 
		FinalDamage, bIsCritical, EnemyStats.CurrentHealth);
	
	return bKilledEnemy;
}

void AWSEnemyBase::Die()
{
	OnDeath();
	
	// Notify game state
	AWSGameState* GameState = Cast<AWSGameState>(GetWorld()->GetGameState());
	if (GameState)
	{
		GameState->EnemyKilled();
	}

	// Award currency to all players who damaged this enemy
	DropCurrency();

	UE_LOG(LogTemp, Log, TEXT("Enemy died: %d"), (int32)EnemyType);

	// Destroy actor
	Destroy();
}

void AWSEnemyBase::AttackTarget()
{
	if (!CurrentTarget)
	{
		return;
	}

	// Deal damage to target
	// This would be implemented in specific enemy classes
	
	UE_LOG(LogTemp, Log, TEXT("Enemy attacking target"));
}

AActor* AWSEnemyBase::FindNearestPlayer()
{
	AActor* NearestPlayer = nullptr;
	float NearestDistance = FLT_MAX;

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), Players);

	for (AActor* Player : Players)
	{
		if (Player && Player != this)
		{
			float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestPlayer = Player;
			}
		}
	}

	return NearestPlayer;
}

AActor* AWSEnemyBase::FindHighestDamageDealer()
{
	AActor* HighestDealer = nullptr;
	float HighestDamage = 0.0f;

	for (const TPair<AActor*, float>& Entry : DamageReceivedFromPlayers)
	{
		if (Entry.Value > HighestDamage)
		{
			HighestDamage = Entry.Value;
			HighestDealer = Entry.Key;
		}
	}

	// Fallback to nearest player if no damage dealers
	if (!HighestDealer)
	{
		return FindNearestPlayer();
	}

	return HighestDealer;
}

void AWSEnemyBase::OnDeath()
{
	// Special death behaviors per enemy type
	switch (EnemyType)
	{
		case EWSEnemyType::Xurkex: // Tank (Death) - Explodes
			// TODO: Create explosion effect and damage nearby players
			UE_LOG(LogTemp, Log, TEXT("Xurkex exploding on death!"));
			break;
			
		case EWSEnemyType::QueenLarvae: // Queen Larvae - Explodes on impact
			UE_LOG(LogTemp, Log, TEXT("Queen Larvae exploding!"));
			break;
			
		default:
			break;
	}
}

void AWSEnemyBase::DropCurrency()
{
	// Award currency to players who damaged this enemy
	for (const TPair<AActor*, float>& Entry : DamageReceivedFromPlayers)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(Entry.Key);
		if (PlayerCharacter)
		{
			APlayerController* PC = Cast<APlayerController>(PlayerCharacter->GetController());
			if (PC)
			{
				AWSPlayerState* PS = Cast<AWSPlayerState>(PC->PlayerState);
				if (PS)
				{
					// Distribute currency based on damage contribution
					float DamagePercent = Entry.Value / EnemyStats.MaxHealth;
					int32 CurrencyAmount = FMath::CeilToInt(EnemyStats.CurrencyDropAmount * DamagePercent);
					PS->AddCurrency(CurrencyAmount);
				}
			}
		}
	}
}
