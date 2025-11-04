// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WSTypes.h"
#include "WSEnemyBase.generated.h"

/**
 * Base class for all enemies
 */
UCLASS()
class WAVESURVIVAL_API AWSEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AWSEnemyBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Enemy configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	EWSEnemyType EnemyType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	FWSEnemyStats EnemyStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemy")
	bool bIsBoss;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	float CriticalDamageMultiplier = 2.0f;

	// Combat
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual bool TakeDamageCustom(float DamageAmount, AActor* DamageCauser, bool bIsCritical);

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void AttackTarget();

	// AI targeting
	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* FindNearestPlayer();

	UFUNCTION(BlueprintCallable, Category = "AI")
	AActor* FindHighestDamageDealer();

	// Health bar
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateHealthBar();

protected:
	UPROPERTY()
	AActor* CurrentTarget;

	UPROPERTY()
	TMap<AActor*, float> DamageReceivedFromPlayers;

	virtual void OnDeath();
	void DropCurrency();
};
