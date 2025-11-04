// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSPlayerState.h"
#include "Net/UnrealNetwork.h"

AWSPlayerState::AWSPlayerState()
{
	CharacterClass = EWSCharacterClass::Rogue;
	CurrentState = EWSPlayerState::Alive;
	
	Currency = 0;
	Kills = 0;
	Deaths = 0;
	Revives = 0;
	DamageDealt = 0.0f;
}

void AWSPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	// Initialize player stats based on character class
	PlayerStats = FWSPlayerStats();
	
	UE_LOG(LogTemp, Log, TEXT("Player State initialized for class %d"), (int32)CharacterClass);
}

void AWSPlayerState::AddCurrency(int32 Amount)
{
	Currency += Amount;
	
	UE_LOG(LogTemp, Log, TEXT("Currency added: +%d (Total: %d)"), Amount, Currency);
}

bool AWSPlayerState::SpendCurrency(int32 Amount)
{
	if (Currency >= Amount)
	{
		Currency -= Amount;
		UE_LOG(LogTemp, Log, TEXT("Currency spent: -%d (Remaining: %d)"), Amount, Currency);
		return true;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Not enough currency: Need %d, Have %d"), Amount, Currency);
	return false;
}

void AWSPlayerState::PurchaseUpgrade(const FWSUpgradeCardData& UpgradeCard)
{
	if (!SpendCurrency(UpgradeCard.Cost))
	{
		return;
	}

	// Add to purchased upgrades
	PurchasedUpgrades.Add(UpgradeCard.CardID);
	
	// Track stacks
	if (UpgradeCard.bStackable)
	{
		if (HasUpgradeStackEntry(UpgradeCard.CardID))
		{
			SetUpgradeStacks(UpgradeCard.CardID, GetUpgradeStacks(UpgradeCard.CardID) + 1);
		}
		else
		{
			SetUpgradeStacks(UpgradeCard.CardID, 1);
		}
	}
	else
	{
		SetUpgradeStacks(UpgradeCard.CardID, 1);
	}

	// Apply upgrade effects
	ApplyUpgradeEffects(UpgradeCard);
	
	UE_LOG(LogTemp, Log, TEXT("Upgrade purchased: %s (Stacks: %d)"), 
		*UpgradeCard.CardName.ToString(), 
		GetUpgradeStacks(UpgradeCard.CardID));
}

bool AWSPlayerState::HasUpgrade(FName UpgradeID) const
{
	return PurchasedUpgrades.Contains(UpgradeID);
}

int32 AWSPlayerState::GetUpgradeStacks(FName UpgradeID) const
{
	int32 Index = UpgradeStackKeys.Find(UpgradeID);
	if (Index != INDEX_NONE && Index < UpgradeStackValues.Num())
	{
		return UpgradeStackValues[Index];
	}
	return 0;
}

void AWSPlayerState::SetUpgradeStacks(FName UpgradeID, int32 Stacks)
{
	int32 Index = UpgradeStackKeys.Find(UpgradeID);
	if (Index != INDEX_NONE)
	{
		// Update existing entry with bounds check
		if (Index < UpgradeStackValues.Num())
		{
			UpgradeStackValues[Index] = Stacks;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UpgradeStacks index out of bounds: Index %d >= Values.Num() %d"), 
				Index, UpgradeStackValues.Num());
		}
	}
	else
	{
		// Add new entry
		UpgradeStackKeys.Add(UpgradeID);
		UpgradeStackValues.Add(Stacks);
	}
}

bool AWSPlayerState::HasUpgradeStackEntry(FName UpgradeID) const
{
	return UpgradeStackKeys.Contains(UpgradeID);
}

void AWSPlayerState::OnKill()
{
	Kills++;
	
	// Award currency per kill
	AddCurrency(10);
}

void AWSPlayerState::OnDeath()
{
	Deaths++;
	CurrentState = EWSPlayerState::Dead;
	
	UE_LOG(LogTemp, Log, TEXT("Player died (Total deaths: %d)"), Deaths);
}

void AWSPlayerState::OnRevive()
{
	Revives++;
	CurrentState = EWSPlayerState::Alive;
	
	// Restore some health on revive
	PlayerStats.CurrentHealth = PlayerStats.MaxHealth * 0.5f;
	
	UE_LOG(LogTemp, Log, TEXT("Player revived (Total revives: %d)"), Revives);
}

void AWSPlayerState::ApplyUpgradeEffects(const FWSUpgradeCardData& UpgradeCard)
{
	// Apply stat modifications based on the upgrade effect
	FName EffectID = UpgradeCard.EffectIdentifier;
	float Value = UpgradeCard.EffectValue;
	
	if (EffectID == "DamageIncrease")
	{
		PlayerStats.DamageMultiplier += Value;
	}
	else if (EffectID == "MaxHealthIncrease")
	{
		PlayerStats.MaxHealth += Value;
		PlayerStats.CurrentHealth += Value;
	}
	else if (EffectID == "AmmoIncrease")
	{
		PlayerStats.MaxAmmo += FMath::CeilToInt(Value);
	}
	else if (EffectID == "ReloadSpeed")
	{
		PlayerStats.ReloadSpeed += Value;
	}
	else if (EffectID == "DamageReduction")
	{
		PlayerStats.DamageReduction += Value;
	}
	else if (EffectID == "HealthRegen")
	{
		PlayerStats.HealthRegenRate += Value;
	}
	else if (EffectID == "CooldownReduction")
	{
		PlayerStats.CooldownReduction += Value;
	}
	else if (EffectID == "CriticalChance")
	{
		PlayerStats.CriticalChance += Value;
	}
	else if (EffectID == "CriticalDamage")
	{
		PlayerStats.CriticalDamageMultiplier += Value;
	}
	else if (EffectID == "MovementSpeed")
	{
		PlayerStats.MovementSpeedMultiplier += Value;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Applied upgrade effect: %s with value %f"), 
		*EffectID.ToString(), Value);
}

void AWSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWSPlayerState, PlayerStats);
	DOREPLIFETIME(AWSPlayerState, CharacterClass);
	DOREPLIFETIME(AWSPlayerState, CurrentState);
	DOREPLIFETIME(AWSPlayerState, Currency);
	DOREPLIFETIME(AWSPlayerState, PurchasedUpgrades);
	DOREPLIFETIME(AWSPlayerState, UpgradeStackKeys);
	DOREPLIFETIME(AWSPlayerState, UpgradeStackValues);
	DOREPLIFETIME(AWSPlayerState, Kills);
	DOREPLIFETIME(AWSPlayerState, Deaths);
	DOREPLIFETIME(AWSPlayerState, Revives);
	DOREPLIFETIME(AWSPlayerState, DamageDealt);
}
