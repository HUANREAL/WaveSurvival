// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSGameState.h"
#include "Net/UnrealNetwork.h"

AWSGameState::AWSGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentWaveNumber = 0;
	RemainingEnemies = 0;
	TotalEnemiesThisWave = 0;
	CurrentPhase = EWSWavePhase::PreWave;
	ShopTimeRemaining = 0.0f;
	ShopPhaseDuration = 45.0f; // 45 seconds shop phase
	
	GameMode = EWSGameMode::MainMode;
	Difficulty = EWSDifficulty::Normal;
	
	ActivePlayerCount = 0;
	TotalKills = 0;
	ElapsedTime = 0.0f;
	
	bGameOver = false;
	bVictory = false;
}

void AWSGameState::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Log, TEXT("Game State initialized"));
}

void AWSGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapsedTime += DeltaTime;

	// Update shop timer
	if (CurrentPhase == EWSWavePhase::PreWave && ShopTimeRemaining > 0.0f)
	{
		ShopTimeRemaining -= DeltaTime;
		if (ShopTimeRemaining <= 0.0f)
		{
			EndShopPhase();
		}
	}
}

void AWSGameState::StartNextWave()
{
	if (bGameOver)
	{
		return;
	}

	CurrentWaveNumber++;
	CurrentPhase = EWSWavePhase::Combat;
	
	// Calculate enemies based on game mode and wave number
	if (GameMode == EWSGameMode::MainMode)
	{
		if (CurrentWaveNumber == 10)
		{
			CurrentPhase = EWSWavePhase::BossFight;
			TotalEnemiesThisWave = 1; // Boss only
		}
		else
		{
			TotalEnemiesThisWave = BaseEnemyCountPerPlayer * ActivePlayerCount;
		}
	}
	else // Survival Mode
	{
		// Scaling formula for survival
		float WaveMultiplier = 1.0f + (CurrentWaveNumber * 0.15f);
		TotalEnemiesThisWave = FMath::CeilToInt(BaseEnemyCountPerPlayer * ActivePlayerCount * WaveMultiplier);
	}
	
	RemainingEnemies = TotalEnemiesThisWave;
	
	UE_LOG(LogTemp, Log, TEXT("Wave %d started - %d enemies"), CurrentWaveNumber, TotalEnemiesThisWave);
}

void AWSGameState::EndCurrentWave()
{
	if (CurrentPhase == EWSWavePhase::BossFight && RemainingEnemies == 0)
	{
		// Victory in Main Mode
		bVictory = true;
		bGameOver = true;
		CurrentPhase = EWSWavePhase::PostWave;
		
		UE_LOG(LogTemp, Log, TEXT("Victory! Boss defeated!"));
	}
	else
	{
		CurrentPhase = EWSWavePhase::PostWave;
		StartShopPhase();
		
		UE_LOG(LogTemp, Log, TEXT("Wave %d completed"), CurrentWaveNumber);
	}
}

void AWSGameState::StartShopPhase()
{
	CurrentPhase = EWSWavePhase::PreWave;
	ShopTimeRemaining = ShopPhaseDuration;
	
	UE_LOG(LogTemp, Log, TEXT("Shop phase started - %f seconds"), ShopPhaseDuration);
}

void AWSGameState::EndShopPhase()
{
	ShopTimeRemaining = 0.0f;
	StartNextWave();
	
	UE_LOG(LogTemp, Log, TEXT("Shop phase ended"));
}

void AWSGameState::EnemyKilled()
{
	RemainingEnemies = FMath::Max(0, RemainingEnemies - 1);
	TotalKills++;
	
	if (RemainingEnemies == 0)
	{
		EndCurrentWave();
	}
}

bool AWSGameState::IsGameOver() const
{
	return bGameOver;
}

bool AWSGameState::IsVictory() const
{
	return bVictory;
}

void AWSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWSGameState, CurrentWaveNumber);
	DOREPLIFETIME(AWSGameState, RemainingEnemies);
	DOREPLIFETIME(AWSGameState, TotalEnemiesThisWave);
	DOREPLIFETIME(AWSGameState, CurrentPhase);
	DOREPLIFETIME(AWSGameState, ShopTimeRemaining);
	DOREPLIFETIME(AWSGameState, GameMode);
	DOREPLIFETIME(AWSGameState, Difficulty);
	DOREPLIFETIME(AWSGameState, ActivePlayerCount);
	DOREPLIFETIME(AWSGameState, TotalKills);
	DOREPLIFETIME(AWSGameState, ElapsedTime);
}
