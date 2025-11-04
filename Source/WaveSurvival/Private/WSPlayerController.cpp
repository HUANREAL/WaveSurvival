// Copyright Epic Games, Inc. All Rights Reserved.

#include "WSPlayerController.h"
#include "WSPlayerState.h"
#include "Blueprint/UserWidget.h"

AWSPlayerController::AWSPlayerController()
{
}

void AWSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	WSPlayerState = Cast<AWSPlayerState>(PlayerState);

	// Create HUD
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidget)
		{
			HUDWidget->AddToViewport();
		}
	}

	InitializeCardDeck();
	
	UE_LOG(LogTemp, Log, TEXT("Player Controller initialized"));
}

void AWSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Input bindings are handled in the Character class
}

void AWSPlayerController::OpenShop()
{
	if (!ShopWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Shop Widget Class assigned!"));
		return;
	}

	if (!ShopWidget)
	{
		ShopWidget = CreateWidget<UUserWidget>(this, ShopWidgetClass);
	}

	if (ShopWidget)
	{
		ShopWidget->AddToViewport(1);
		
		// Show mouse cursor
		bShowMouseCursor = true;
		SetInputMode(FInputModeGameAndUI());
		
		UE_LOG(LogTemp, Log, TEXT("Shop opened"));
	}
}

void AWSPlayerController::CloseShop()
{
	if (ShopWidget)
	{
		ShopWidget->RemoveFromParent();
		
		// Hide mouse cursor
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		
		UE_LOG(LogTemp, Log, TEXT("Shop closed"));
	}
}

void AWSPlayerController::DrawCard()
{
	if (AvailableCards.Num() == 0)
	{
		// Reshuffle if deck is empty
		UE_LOG(LogTemp, Log, TEXT("Deck empty - reshuffling"));
		InitializeCardDeck();
	}

	CurrentCard = GetNextCard();
	DrawnCards.Add(CurrentCard);
	
	UE_LOG(LogTemp, Log, TEXT("Drew card: %s"), *CurrentCard.CardName.ToString());
}

void AWSPlayerController::PurchaseCard(const FWSUpgradeCardData& Card)
{
	if (!WSPlayerState)
	{
		return;
	}

	if (WSPlayerState->Currency >= Card.Cost)
	{
		WSPlayerState->PurchaseUpgrade(Card);
		
		// Remove from available cards
		AvailableCards.Remove(Card);
		
		UE_LOG(LogTemp, Log, TEXT("Purchased card: %s for %d currency"), 
			*Card.CardName.ToString(), Card.Cost);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough currency to purchase card"));
	}
}

void AWSPlayerController::DestroyCard(const FWSUpgradeCardData& Card)
{
	// Remove from available cards permanently
	AvailableCards.Remove(Card);
	DestroyedCards.Add(Card);
	
	UE_LOG(LogTemp, Log, TEXT("Destroyed card: %s"), *Card.CardName.ToString());
}

void AWSPlayerController::HoldCard()
{
	UE_LOG(LogTemp, Log, TEXT("Holding card for later"));
	// Card remains in the deck for future draws
}

void AWSPlayerController::InitializeCardDeck()
{
	AvailableCards.Empty();
	
	if (!WSPlayerState)
	{
		return;
	}

	// Create deck with approximately 1/3 generic and 2/3 character-specific cards
	
	// Generic cards (10 types)
	TArray<FName> GenericUpgrades = {
		"DamageIncrease",
		"MaxHealthIncrease",
		"AmmoIncrease",
		"ReloadSpeed",
		"DamageReduction",
		"HealthRegen",
		"CooldownReduction",
		"CriticalChance",
		"CriticalDamage",
		"MovementSpeed"
	};

	for (int32 i = 0; i < 15; i++) // 15 generic cards
	{
		FWSUpgradeCardData Card;
		FName UpgradeType = GenericUpgrades[i % GenericUpgrades.Num()];
		Card.CardID = FName(*FString::Printf(TEXT("Generic_%s_%d"), *UpgradeType.ToString(), i));
		Card.CardName = FText::FromName(UpgradeType);
		Card.CardDescription = FText::FromString(TEXT("Generic upgrade for all characters"));
		Card.CardType = EWSUpgradeCardType::Generic;
		Card.Rarity = EWSCardRarity::Common;
		Card.Cost = 100;
		Card.bStackable = true;
		Card.MaxStacks = 10;
		Card.EffectValue = 0.1f;
		Card.EffectIdentifier = UpgradeType;
		
		AvailableCards.Add(Card);
	}

	// Character-specific cards (30 cards)
	for (int32 i = 0; i < 30; i++)
	{
		FWSUpgradeCardData Card;
		Card.CardID = FName(*FString::Printf(TEXT("CharacterSpecific_%d"), i));
		Card.CardName = FText::FromString(TEXT("Character Specific Upgrade"));
		Card.CardDescription = FText::FromString(TEXT("Upgrade specific to your character"));
		Card.CardType = EWSUpgradeCardType::CharacterSpecific;
		Card.Rarity = EWSCardRarity::Uncommon;
		Card.SpecificClass = WSPlayerState->CharacterClass;
		Card.Cost = 150;
		Card.bStackable = true;
		Card.MaxStacks = 5;
		Card.EffectValue = 0.15f;
		Card.EffectIdentifier = "CharacterBonus";
		
		AvailableCards.Add(Card);
	}

	// Legendary cards (3 cards)
	for (int32 i = 0; i < 3; i++)
	{
		FWSUpgradeCardData Card;
		Card.CardID = FName(*FString::Printf(TEXT("Legendary_%d"), i));
		Card.CardName = FText::FromString(TEXT("Legendary Upgrade"));
		Card.CardDescription = FText::FromString(TEXT("Powerful legendary upgrade"));
		Card.CardType = EWSUpgradeCardType::Legendary;
		Card.Rarity = EWSCardRarity::Legendary;
		Card.SpecificClass = WSPlayerState->CharacterClass;
		Card.Cost = 500;
		Card.bStackable = false;
		Card.MaxStacks = 1;
		Card.EffectValue = 0.5f;
		Card.EffectIdentifier = "LegendaryBonus";
		
		AvailableCards.Add(Card);
	}

	ShuffleCardDeck();
	
	UE_LOG(LogTemp, Log, TEXT("Card deck initialized with %d cards"), AvailableCards.Num());
}

void AWSPlayerController::ShuffleCardDeck()
{
	// Fisher-Yates shuffle
	for (int32 i = AvailableCards.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		AvailableCards.Swap(i, j);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Card deck shuffled"));
}

FWSUpgradeCardData AWSPlayerController::GetNextCard()
{
	if (AvailableCards.Num() > 0)
	{
		FWSUpgradeCardData Card = AvailableCards[0];
		AvailableCards.RemoveAt(0);
		return Card;
	}
	
	return FWSUpgradeCardData();
}
