// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "WSTypes.h"
#include "WSGameInstance.generated.h"

/**
 * Game Instance handles persistent data across maps and sessions
 */
UCLASS()
class WAVESURVIVAL_API UWSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UWSGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	// Game mode selection
	UPROPERTY(BlueprintReadWrite, Category = "Game Settings")
	EWSGameMode CurrentGameMode;

	UPROPERTY(BlueprintReadWrite, Category = "Game Settings")
	EWSDifficulty SelectedDifficulty;

	// Player progression (persistent across runs)
	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	int32 PlayerLevel;

	UPROPERTY(BlueprintReadWrite, Category = "Progression")
	TMap<FName, int32> UnlockedCosmetics;

	// Online subsystem
	UFUNCTION(BlueprintCallable, Category = "Online")
	void InitializeOnlineSubsystem();

	UFUNCTION(BlueprintCallable, Category = "Online")
	void CreateSession(int32 MaxPlayers);

	UFUNCTION(BlueprintCallable, Category = "Online")
	void FindSessions();

	UFUNCTION(BlueprintCallable, Category = "Online")
	void JoinSession(int32 SessionIndex);

	// Leaderboard
	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	void SubmitLeaderboardScore(int32 WaveReached, EWSCharacterClass CharacterClass, int32 TotalKills);

	UFUNCTION(BlueprintCallable, Category = "Leaderboard")
	TArray<FWSLeaderboardEntry> GetLeaderboard();

protected:
	UPROPERTY()
	TArray<FWSLeaderboardEntry> LeaderboardData;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
