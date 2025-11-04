// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"

UWSGameInstance::UWSGameInstance()
{
	CurrentGameMode = EWSGameMode::MainMode;
	SelectedDifficulty = EWSDifficulty::Normal;
	PlayerLevel = 1;
}

void UWSGameInstance::Init()
{
	Super::Init();
	
	UE_LOG(LogTemp, Log, TEXT("Wave Survival Game Instance Initialized"));
	
	InitializeOnlineSubsystem();
}

void UWSGameInstance::Shutdown()
{
	Super::Shutdown();
	
	UE_LOG(LogTemp, Log, TEXT("Wave Survival Game Instance Shutdown"));
}

void UWSGameInstance::InitializeOnlineSubsystem()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	
	if (OnlineSubsystem)
	{
		UE_LOG(LogTemp, Log, TEXT("Online Subsystem: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UWSGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UWSGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UWSGameInstance::OnJoinSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Online Subsystem found!"));
	}
}

void UWSGameInstance::CreateSession(int32 MaxPlayers)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create session - no online subsystem"));
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot create session - invalid session interface"));
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = MaxPlayers;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinInProgress = false;
	SessionSettings.bIsLANMatch = false;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;

	SessionInterface->CreateSession(0, FName("WaveSurvivalSession"), SessionSettings);
	
	UE_LOG(LogTemp, Log, TEXT("Creating session for %d players"), MaxPlayers);
}

void UWSGameInstance::FindSessions()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}

	TSharedRef<FOnlineSessionSearch> SearchSettings = MakeShareable(new FOnlineSessionSearch());
	SearchSettings->bIsLanQuery = false;
	SearchSettings->MaxSearchResults = 20;
	SearchSettings->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SearchSettings);
	
	UE_LOG(LogTemp, Log, TEXT("Searching for sessions"));
}

void UWSGameInstance::JoinSession(int32 SessionIndex)
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}

	IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}

	// This would need to reference the search results from FindSessions
	// For now, this is a placeholder
	UE_LOG(LogTemp, Log, TEXT("Joining session at index %d"), SessionIndex);
}

void UWSGameInstance::SubmitLeaderboardScore(int32 WaveReached, EWSCharacterClass CharacterClass, int32 TotalKills)
{
	FWSLeaderboardEntry Entry;
	Entry.PlayerName = TEXT("Player"); // Would get actual player name
	Entry.HighestWave = WaveReached;
	Entry.CharacterClass = CharacterClass;
	Entry.TotalKills = TotalKills;
	Entry.CompletionDate = FDateTime::Now();

	LeaderboardData.Add(Entry);

	// Sort by highest wave
	LeaderboardData.Sort([](const FWSLeaderboardEntry& A, const FWSLeaderboardEntry& B) {
		return A.HighestWave > B.HighestWave;
	});

	// Keep only top 100
	if (LeaderboardData.Num() > 100)
	{
		LeaderboardData.SetNum(100);
	}

	UE_LOG(LogTemp, Log, TEXT("Leaderboard score submitted: Wave %d, Class %d, Kills %d"), 
		WaveReached, (int32)CharacterClass, TotalKills);
}

TArray<FWSLeaderboardEntry> UWSGameInstance::GetLeaderboard()
{
	return LeaderboardData;
}

void UWSGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session created successfully: %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session: %s"), *SessionName.ToString());
	}
}

void UWSGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Sessions found successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find sessions"));
	}
}

void UWSGameInstance::OnJoinSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined session successfully: %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session: %s"), *SessionName.ToString());
	}
}
