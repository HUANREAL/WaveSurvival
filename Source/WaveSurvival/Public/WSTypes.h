// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WSTypes.generated.h"

/**
 * Game mode types
 */
UENUM(BlueprintType)
enum class EWSGameMode : uint8
{
	MainMode UMETA(DisplayName = "Main Mode (10 Waves)"),
	SurvivalMode UMETA(DisplayName = "Survival Mode (Endless)")
};

/**
 * Difficulty levels for Main Mode
 */
UENUM(BlueprintType)
enum class EWSDifficulty : uint8
{
	Easy UMETA(DisplayName = "Easy"),
	Normal UMETA(DisplayName = "Normal"),
	Hard UMETA(DisplayName = "Hard"),
	Extreme UMETA(DisplayName = "Extreme")
};

/**
 * Player character classes
 */
UENUM(BlueprintType)
enum class EWSCharacterClass : uint8
{
	Rogue UMETA(DisplayName = "Rogue - Arkhrin"),
	Assault UMETA(DisplayName = "Assault - Elite Trooper"),
	RangedSpecialist UMETA(DisplayName = "Ranged Specialist - Formiops"),
	HeavyGunner UMETA(DisplayName = "Heavy Gunner - Gastarian"),
	StaffCombat UMETA(DisplayName = "Staff Combat - Mallearian Knight"),
	Medic UMETA(DisplayName = "Medic - Juggernaut")
};

/**
 * Enemy types
 */
UENUM(BlueprintType)
enum class EWSEnemyType : uint8
{
	// Basic Enemies
	Aalix UMETA(DisplayName = "Aalix (Standard Grunt)"),
	Krals UMETA(DisplayName = "Krals (Spider)"),
	Xurkex UMETA(DisplayName = "Xurkex (Tank Death)"),
	Crelmor UMETA(DisplayName = "Crelmor (Tank Armored)"),
	Gid UMETA(DisplayName = "Gid (Fast Glass Cannon)"),
	
	// Bosses
	Vruuknuix UMETA(DisplayName = "Vruuknuix (The Queen)"),
	Saarlae UMETA(DisplayName = "Saarlae (The Squid)"),
	
	// Boss Minions
	QueenLarvae UMETA(DisplayName = "Queen Larvae")
};

/**
 * Elemental types
 */
UENUM(BlueprintType)
enum class EWSElementalType : uint8
{
	None UMETA(DisplayName = "None"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice"),
	Acid UMETA(DisplayName = "Acid"),
	Electric UMETA(DisplayName = "Electric"),
	Poison UMETA(DisplayName = "Poison")
};

/**
 * Card rarity
 */
UENUM(BlueprintType)
enum class EWSCardRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Legendary UMETA(DisplayName = "Legendary")
};

/**
 * Upgrade card types
 */
UENUM(BlueprintType)
enum class EWSUpgradeCardType : uint8
{
	Generic UMETA(DisplayName = "Generic (All Characters)"),
	CharacterSpecific UMETA(DisplayName = "Character Specific"),
	Legendary UMETA(DisplayName = "Legendary")
};

/**
 * Upgrade card categories
 */
UENUM(BlueprintType)
enum class EWSUpgradeCategory : uint8
{
	WeaponStats UMETA(DisplayName = "Weapon Stats"),
	PassiveStats UMETA(DisplayName = "Passive Stats"),
	SkillEnhancement UMETA(DisplayName = "Skill Enhancement"),
	UltimateEnhancement UMETA(DisplayName = "Ultimate Enhancement")
};

/**
 * Player state enum
 */
UENUM(BlueprintType)
enum class EWSPlayerState : uint8
{
	Alive UMETA(DisplayName = "Alive"),
	Downed UMETA(DisplayName = "Downed"),
	Dead UMETA(DisplayName = "Dead"),
	Respawning UMETA(DisplayName = "Respawning")
};

/**
 * Wave phase
 */
UENUM(BlueprintType)
enum class EWSWavePhase : uint8
{
	PreWave UMETA(DisplayName = "Pre-Wave (Shop)"),
	Combat UMETA(DisplayName = "Combat"),
	PostWave UMETA(DisplayName = "Post-Wave"),
	BossFight UMETA(DisplayName = "Boss Fight")
};

/**
 * Boss phase for multi-phase bosses
 */
UENUM(BlueprintType)
enum class EWSBossPhase : uint8
{
	Phase1 UMETA(DisplayName = "Phase 1"),
	Phase2 UMETA(DisplayName = "Phase 2"),
	Phase3 UMETA(DisplayName = "Phase 3"),
	Defeated UMETA(DisplayName = "Defeated")
};

/**
 * Weapon types
 */
UENUM(BlueprintType)
enum class EWSWeaponType : uint8
{
	Shotgun UMETA(DisplayName = "Shotgun"),
	AssaultPistol UMETA(DisplayName = "Assault Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	GatlingGun UMETA(DisplayName = "Gatling Gun"),
	Staff UMETA(DisplayName = "Staff"),
	LightningGun UMETA(DisplayName = "Lightning Gun")
};

/**
 * Weapon fire mode
 */
UENUM(BlueprintType)
enum class EWSWeaponFireMode : uint8
{
	Hitscan UMETA(DisplayName = "Hitscan"),
	Projectile UMETA(DisplayName = "Projectile"),
	Melee UMETA(DisplayName = "Melee")
};

/**
 * Upgrade card data structure
 */
USTRUCT(BlueprintType)
struct FWSUpgradeCardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CardID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CardDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWSUpgradeCardType CardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWSCardRarity Rarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWSUpgradeCategory Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWSCharacterClass SpecificClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStacks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName EffectIdentifier;

	FWSUpgradeCardData()
		: CardType(EWSUpgradeCardType::Generic)
		, Rarity(EWSCardRarity::Common)
		, Category(EWSUpgradeCategory::WeaponStats)
		, Cost(100)
		, SpecificClass(EWSCharacterClass::Rogue)
		, bStackable(true)
		, MaxStacks(10)
		, EffectValue(0.1f)
	{
	}
};

/**
 * Wave configuration data
 */
USTRUCT(BlueprintType)
struct FWSWaveConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaveNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseEnemyCountPerPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EWSEnemyType, float> EnemyComposition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DifficultyMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBossWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWSEnemyType BossType;

	FWSWaveConfig()
		: WaveNumber(1)
		, BaseEnemyCountPerPlayer(250)
		, DifficultyMultiplier(1.0f)
		, bIsBossWave(false)
		, BossType(EWSEnemyType::Vruuknuix)
	{
	}
};

/**
 * Player stats structure
 */
USTRUCT(BlueprintType)
struct FWSPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageReduction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CriticalDamageMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReloadSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownReduction;

	FWSPlayerStats()
		: MaxHealth(100.0f)
		, CurrentHealth(100.0f)
		, DamageMultiplier(1.0f)
		, MovementSpeedMultiplier(1.0f)
		, DamageReduction(0.0f)
		, CriticalChance(0.05f)
		, CriticalDamageMultiplier(2.0f)
		, HealthRegenRate(0.0f)
		, MaxAmmo(30)
		, CurrentAmmo(30)
		, ReloadSpeed(1.0f)
		, CooldownReduction(0.0f)
	{
	}
};

/**
 * Enemy stats structure
 */
USTRUCT(BlueprintType)
struct FWSEnemyStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanCrit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWSElementalType ElementalType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrencyDropAmount;

	FWSEnemyStats()
		: MaxHealth(100.0f)
		, CurrentHealth(100.0f)
		, BaseDamage(10.0f)
		, MovementSpeed(400.0f)
		, Armor(0.0f)
		, bCanCrit(true)
		, ElementalType(EWSElementalType::None)
		, CurrencyDropAmount(10)
	{
	}
};

/**
 * Leaderboard entry
 */
USTRUCT(BlueprintType)
struct FWSLeaderboardEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite)
	int32 HighestWave;

	UPROPERTY(BlueprintReadWrite)
	EWSCharacterClass CharacterClass;

	UPROPERTY(BlueprintReadWrite)
	int32 TotalKills;

	UPROPERTY(BlueprintReadWrite)
	FDateTime CompletionDate;

	FWSLeaderboardEntry()
		: HighestWave(0)
		, CharacterClass(EWSCharacterClass::Rogue)
		, TotalKills(0)
	{
	}
};
