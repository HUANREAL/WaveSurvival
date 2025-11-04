// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSGameMode.h"
#include "WSGameState.h"
#include "WSPlayerController.h"
#include "WSPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

AWSGameMode::AWSGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	GameStateClass = AWSGameState::StaticClass();
	PlayerControllerClass = AWSPlayerController::StaticClass();
	PlayerStateClass = AWSPlayerState::StaticClass();

	SpawnRadius = 2000.0f;
}

void AWSGameMode::BeginPlay()
{
	Super::BeginPlay();

	WSGameState = Cast<AWSGameState>(GameState);
	
	SetupWaveConfigurations();
	
	UE_LOG(LogTemp, Log, TEXT("Game Mode started"));
}

void AWSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWSGameMode::InitializeGame(EWSGameMode InGameMode, EWSDifficulty InDifficulty)
{
	if (!WSGameState)
	{
		return;
	}

	WSGameState->GameMode = InGameMode;
	WSGameState->Difficulty = InDifficulty;
	
	// Count active players
	int32 PlayerCount = 0;
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (It->IsValid())
		{
			PlayerCount++;
		}
	}
	WSGameState->ActivePlayerCount = FMath::Max(1, PlayerCount);

	// Start first shop phase
	WSGameState->StartShopPhase();
	
	UE_LOG(LogTemp, Log, TEXT("Game initialized - Mode: %d, Difficulty: %d, Players: %d"), 
		(int32)InGameMode, (int32)InDifficulty, WSGameState->ActivePlayerCount);
}

void AWSGameMode::SpawnWaveEnemies()
{
	if (!WSGameState)
	{
		return;
	}

	FWSWaveConfig WaveConfig = GetWaveConfig(WSGameState->CurrentWaveNumber);
	
	if (WaveConfig.bIsBossWave)
	{
		// Spawn boss at dedicated location
		FVector BossLocation = GetBossSpawnLocation();
		SpawnEnemy(WaveConfig.BossType, BossLocation);
		
		UE_LOG(LogTemp, Log, TEXT("Spawning boss: %d"), (int32)WaveConfig.BossType);
	}
	else
	{
		// Spawn regular enemies based on composition
		int32 TotalEnemiesToSpawn = WSGameState->TotalEnemiesThisWave;
		
		for (const TPair<EWSEnemyType, float>& Composition : WaveConfig.EnemyComposition)
		{
			int32 EnemyCount = FMath::CeilToInt(TotalEnemiesToSpawn * Composition.Value);
			
			for (int32 i = 0; i < EnemyCount; i++)
			{
				FVector SpawnLocation = GetRandomSpawnLocation();
				SpawnEnemy(Composition.Key, SpawnLocation);
			}
		}
		
		UE_LOG(LogTemp, Log, TEXT("Spawned %d enemies for wave %d"), 
			TotalEnemiesToSpawn, WSGameState->CurrentWaveNumber);
	}
}

void AWSGameMode::SpawnEnemy(EWSEnemyType EnemyType, FVector SpawnLocation)
{
	if (!EnemyClasses.Contains(EnemyType))
	{
		UE_LOG(LogTemp, Warning, TEXT("No class defined for enemy type %d"), (int32)EnemyType);
		return;
	}

	TSubclassOf<class AWSEnemyBase> EnemyClass = EnemyClasses[EnemyType];
	
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid enemy class for type %d"), (int32)EnemyType);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
}

void AWSGameMode::PlayerDowned(AController* Player)
{
	UE_LOG(LogTemp, Log, TEXT("Player downed"));
	
	// Player has limited time to be revived
	CheckGameOverCondition();
}

void AWSGameMode::PlayerRevived(AController* Player)
{
	UE_LOG(LogTemp, Log, TEXT("Player revived"));
}

void AWSGameMode::RespawnPlayer(AController* Player)
{
	if (!Player)
	{
		return;
	}

	FVector RespawnLocation = GetSafeRespawnLocation();
	
	// Spawn player from UFO drop
	// This would trigger the falling animation
	
	UE_LOG(LogTemp, Log, TEXT("Player respawning at %s"), *RespawnLocation.ToString());
}

void AWSGameMode::CheckGameOverCondition()
{
	if (!WSGameState)
	{
		return;
	}

	// Check if all players are dead/downed
	bool bAllPlayersDead = true;
	
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AWSPlayerController* PC = Cast<AWSPlayerController>(*It);
		if (PC)
		{
			AWSPlayerState* PS = Cast<AWSPlayerState>(PC->PlayerState);
			if (PS && PS->CurrentState != EWSPlayerState::Dead && PS->CurrentState != EWSPlayerState::Downed)
			{
				bAllPlayersDead = false;
				break;
			}
		}
	}

	if (bAllPlayersDead)
	{
		// Game Over
		UE_LOG(LogTemp, Log, TEXT("Game Over - All players dead"));
	}
}

FVector AWSGameMode::GetRandomSpawnLocation()
{
	// Get random spawn point
	// For MVP, use a simple radius-based spawn around map center
	
	FVector MapCenter = FVector::ZeroVector;
	
	float Angle = FMath::FRandRange(0.0f, 360.0f);
	float Distance = FMath::FRandRange(500.0f, SpawnRadius);
	
	FVector SpawnLocation = MapCenter + FVector(
		FMath::Cos(FMath::DegreesToRadians(Angle)) * Distance,
		FMath::Sin(FMath::DegreesToRadians(Angle)) * Distance,
		100.0f // Height offset
	);
	
	return SpawnLocation;
}

FVector AWSGameMode::GetBossSpawnLocation()
{
	// Boss spawns at map center for dramatic entrance
	// This provides a predictable, dramatic spawn point for boss encounters
	FVector MapCenter = FVector::ZeroVector;
	
	// Slightly elevated for visual impact
	MapCenter.Z = 100.0f;
	
	return MapCenter;
}

FVector AWSGameMode::GetSafeRespawnLocation()
{
	// Find location with fewest enemies nearby
	// For MVP, return a location far from enemies
	
	return GetRandomSpawnLocation();
}

void AWSGameMode::SetupWaveConfigurations()
{
	// Setup Main Mode wave configurations (Waves 1-10)
	MainModeWaveConfigs.Empty();
	
	for (int32 i = 1; i <= 9; i++)
	{
		FWSWaveConfig Config;
		Config.WaveNumber = i;
		Config.BaseEnemyCountPerPlayer = BaseEnemyCountPerPlayer;
		Config.DifficultyMultiplier = 1.0f + (i * 0.1f);
		Config.bIsBossWave = false;
		
		// Enemy composition changes over waves
		if (i <= 3)
		{
			Config.EnemyComposition.Add(EWSEnemyType::Aalix, 0.9f);
			Config.EnemyComposition.Add(EWSEnemyType::Krals, 0.1f);
		}
		else if (i <= 6)
		{
			Config.EnemyComposition.Add(EWSEnemyType::Aalix, 0.6f);
			Config.EnemyComposition.Add(EWSEnemyType::Krals, 0.2f);
			Config.EnemyComposition.Add(EWSEnemyType::Xurkex, 0.1f);
			Config.EnemyComposition.Add(EWSEnemyType::Gid, 0.1f);
		}
		else
		{
			Config.EnemyComposition.Add(EWSEnemyType::Aalix, 0.4f);
			Config.EnemyComposition.Add(EWSEnemyType::Krals, 0.2f);
			Config.EnemyComposition.Add(EWSEnemyType::Xurkex, 0.15f);
			Config.EnemyComposition.Add(EWSEnemyType::Crelmor, 0.1f);
			Config.EnemyComposition.Add(EWSEnemyType::Gid, 0.15f);
		}
		
		MainModeWaveConfigs.Add(Config);
	}
	
	// Wave 10 - Boss
	FWSWaveConfig BossConfig;
	BossConfig.WaveNumber = 10;
	BossConfig.BaseEnemyCountPerPlayer = 1;
	BossConfig.DifficultyMultiplier = 3.0f;
	BossConfig.bIsBossWave = true;
	BossConfig.BossType = EWSEnemyType::Vruuknuix; // The Queen
	MainModeWaveConfigs.Add(BossConfig);
}

FWSWaveConfig AWSGameMode::GetWaveConfig(int32 WaveNumber)
{
	if (!WSGameState)
	{
		return FWSWaveConfig();
	}

	if (WSGameState->GameMode == EWSGameMode::MainMode)
	{
		if (WaveNumber > 0 && WaveNumber <= MainModeWaveConfigs.Num())
		{
			return MainModeWaveConfigs[WaveNumber - 1];
		}
		// If in MainMode but WaveNumber is invalid, return default config
		return FWSWaveConfig();
	}
	else // Survival Mode
	{
		FWSWaveConfig SurvivalConfig;
		GenerateSurvivalWaveConfig(WaveNumber, SurvivalConfig);
		return SurvivalConfig;
	}
	
	return FWSWaveConfig();
}

void AWSGameMode::GenerateSurvivalWaveConfig(int32 WaveNumber, FWSWaveConfig& OutConfig)
{
	OutConfig.WaveNumber = WaveNumber;
	OutConfig.BaseEnemyCountPerPlayer = BaseEnemyCountPerPlayer;
	OutConfig.DifficultyMultiplier = 1.0f + (WaveNumber * 0.15f);
	OutConfig.bIsBossWave = (WaveNumber % 10 == 0); // Boss every 10 waves
	
	if (OutConfig.bIsBossWave)
	{
		// Alternate between bosses
		OutConfig.BossType = (WaveNumber / 10) % 2 == 1 ? 
			EWSEnemyType::Vruuknuix : EWSEnemyType::Saarlae;
	}
	else
	{
		// Progressive enemy composition
		float AalixPercent = FMath::Max(0.2f, 0.9f - (WaveNumber * 0.05f));
		float KralsPercent = FMath::Min(0.3f, 0.1f + (WaveNumber * 0.02f));
		float XurkexPercent = FMath::Min(0.2f, WaveNumber > 5 ? 0.1f + (WaveNumber * 0.01f) : 0.0f);
		float CrelmorPercent = FMath::Min(0.15f, WaveNumber > 8 ? 0.05f + (WaveNumber * 0.01f) : 0.0f);
		float GidPercent = FMath::Min(0.25f, WaveNumber > 3 ? 0.1f + (WaveNumber * 0.015f) : 0.0f);
		
		// Normalize
		float Total = AalixPercent + KralsPercent + XurkexPercent + CrelmorPercent + GidPercent;
		
		OutConfig.EnemyComposition.Add(EWSEnemyType::Aalix, AalixPercent / Total);
		OutConfig.EnemyComposition.Add(EWSEnemyType::Krals, KralsPercent / Total);
		OutConfig.EnemyComposition.Add(EWSEnemyType::Xurkex, XurkexPercent / Total);
		OutConfig.EnemyComposition.Add(EWSEnemyType::Crelmor, CrelmorPercent / Total);
		OutConfig.EnemyComposition.Add(EWSEnemyType::Gid, GidPercent / Total);
	}
}
