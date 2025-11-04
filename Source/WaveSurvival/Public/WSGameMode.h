// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "WSTypes.h"
#include "WSEnemyBase.h"
#include "WSGameMode.generated.h"

class AWSGameState;
class AWSPlayerController;

/**
 * Game Mode handles game rules and spawning logic
 */
UCLASS()
class WAVESURVIVAL_API AWSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AWSGameMode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Wave management
	UFUNCTION(BlueprintCallable, Category = "Wave")
	void InitializeGame(EWSGameMode InGameMode, EWSDifficulty InDifficulty);

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SpawnWaveEnemies();

	UFUNCTION(BlueprintCallable, Category = "Wave")
	void SpawnEnemy(EWSEnemyType EnemyType, FVector SpawnLocation);

	// Player management
	UFUNCTION(BlueprintCallable, Category = "Player")
	void PlayerDowned(AController* Player);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void PlayerRevived(AController* Player);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void RespawnPlayer(AController* Player);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void CheckGameOverCondition();

	// Spawn points
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomSpawnLocation();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetBossSpawnLocation();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetSafeRespawnLocation();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	TArray<FWSWaveConfig> MainModeWaveConfigs;

	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	int32 BaseEnemyCountPerPlayer = 250;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TMap<EWSEnemyType, TSubclassOf<AWSEnemyBase>> EnemyClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TArray<AActor*> EnemySpawnPoints;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float SpawnRadius;

	UPROPERTY()
	AWSGameState* WSGameState;

	void SetupWaveConfigurations();
	FWSWaveConfig GetWaveConfig(int32 WaveNumber);
	void GenerateSurvivalWaveConfig(int32 WaveNumber, FWSWaveConfig& OutConfig);
};
