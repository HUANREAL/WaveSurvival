// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "WSTypes.h"
#include "WSGameState.generated.h"

/**
 * Game State tracks the current state of the match
 */
UCLASS()
class WAVESURVIVAL_API AWSGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AWSGameState();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Wave management
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Wave")
	int32 CurrentWaveNumber;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Wave")
	int32 RemainingEnemies;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Wave")
	int32 TotalEnemiesThisWave;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Wave")
	EWSWavePhase CurrentPhase;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Wave")
	float ShopTimeRemaining;

	UPROPERTY(BlueprintReadWrite, Category = "Wave")
	float ShopPhaseDuration;

	// Game mode and difficulty
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Game")
	EWSGameMode GameMode;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Game")
	EWSDifficulty Difficulty;

	// Active players
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Players")
	int32 ActivePlayerCount;

	// Statistics
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Stats")
	int32 TotalKills;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Stats")
	float ElapsedTime;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartNextWave();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void EndCurrentWave();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void StartShopPhase();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void EndShopPhase();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void EnemyKilled();

	UFUNCTION(BlueprintCallable, Category = "Game")
	bool IsGameOver() const;

	UFUNCTION(BlueprintCallable, Category = "Game")
	bool IsVictory() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	bool bGameOver;
	bool bVictory;
};
