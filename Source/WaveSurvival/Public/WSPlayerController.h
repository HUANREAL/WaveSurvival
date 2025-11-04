// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WSTypes.h"
#include "WSPlayerController.generated.h"

class AWSPlayerState;
class UWSShopWidget;

/**
 * Player Controller handles input and UI
 */
UCLASS()
class WAVESURVIVAL_API AWSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AWSPlayerController();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Shop UI
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OpenShop();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseShop();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void DrawCard();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void PurchaseCard(const FWSUpgradeCardData& Card);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void DestroyCard(const FWSUpgradeCardData& Card);

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void HoldCard();

	// HUD
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateHUD();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowDeathScreen();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowVictoryScreen();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowGameOverScreen();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ShopWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

	UPROPERTY()
	UUserWidget* ShopWidget;

	UPROPERTY()
	UUserWidget* HUDWidget;

	UPROPERTY()
	AWSPlayerState* WSPlayerState;

	// Current card deck
	UPROPERTY()
	TArray<FWSUpgradeCardData> AvailableCards;

	UPROPERTY()
	TArray<FWSUpgradeCardData> DrawnCards;

	UPROPERTY()
	TArray<FWSUpgradeCardData> DestroyedCards;

	FWSUpgradeCardData CurrentCard;

	void InitializeCardDeck();
	void ShuffleCardDeck();
	FWSUpgradeCardData GetNextCard();
};
