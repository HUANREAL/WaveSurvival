// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "WSTypes.h"
#include "WSPlayerState.generated.h"

/**
 * Player State tracks individual player data
 */
UCLASS()
class WAVESURVIVAL_API AWSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AWSPlayerState();

	virtual void BeginPlay() override;

	// Player stats
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	FWSPlayerStats PlayerStats;

	// Character class
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Character")
	EWSCharacterClass CharacterClass;

	// Player state
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "State")
	EWSPlayerState CurrentState;

	// Currency
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Economy")
	int32 Currency;

	// Upgrades
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Upgrades")
	TArray<FName> PurchasedUpgrades;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Upgrades")
	TMap<FName, int32> UpgradeStacks;

	// Statistics
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Kills;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Deaths;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	int32 Revives;

	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Stats")
	float DamageDealt;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Economy")
	void AddCurrency(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Economy")
	bool SpendCurrency(int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	void PurchaseUpgrade(const FWSUpgradeCardData& UpgradeCard);

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	bool HasUpgrade(FName UpgradeID) const;

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	int32 GetUpgradeStacks(FName UpgradeID) const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void OnKill();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void OnDeath();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void OnRevive();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void ApplyUpgradeEffects(const FWSUpgradeCardData& UpgradeCard);
};
