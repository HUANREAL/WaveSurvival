// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WSTypes.h"
#include "WSCharacterBase.generated.h"

class AWSPlayerState;
class AWSWeaponBase;

/**
 * Base class for all player characters
 */
UCLASS()
class WAVESURVIVAL_API AWSCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AWSCharacterBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Character class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
	EWSCharacterClass CharacterClass;

	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AWSWeaponBase> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWSWeaponBase* CurrentWeapon;

	// Abilities
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	float Ability1Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	float Ability2Cooldown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Abilities")
	float UltimateCooldown;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	float Ability1RemainingCooldown;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	float Ability2RemainingCooldown;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	float UltimateRemainingCooldown;

	// Combat
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void TakeDamageCustom(float DamageAmount, AActor* DamageCauser);

	// Abilities
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void UseAbility1();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void UseAbility2();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void UseUltimate();

	// State
	UFUNCTION(BlueprintCallable, Category = "State")
	void Revive();

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsDowned() const;

protected:
	// Input handlers
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void StartJump();
	void StopJump();

	UPROPERTY()
	AWSPlayerState* WSPlayerState;

	bool bIsFiring;
	bool bIsReloading;
	bool bIsDowned;

	void UpdateCooldowns(float DeltaTime);
	void ApplyHealthRegen(float DeltaTime);
};
