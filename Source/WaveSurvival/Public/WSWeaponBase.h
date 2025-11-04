// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WSTypes.h"
#include "WSWeaponBase.generated.h"

/**
 * Base class for all weapons
 */
UCLASS()
class WAVESURVIVAL_API AWSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWSWeaponBase();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Weapon configuration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	EWSWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	EWSWeaponFireMode FireMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 MagazineSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float Range;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool bAutomatic;

	// Ammo
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int32 CurrentAmmo;

	// Elemental damage
	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	EWSElementalType ElementalType;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	float ElementalDamagePercent;

	// Firing
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool CanFire() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool NeedsReload() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	bool bIsFiring;
	bool bIsReloading;
	float TimeSinceLastShot;
	FTimerHandle ReloadTimerHandle;

	virtual void PerformHitscan();
	virtual void PerformProjectile();
	virtual void PerformMelee();
	
	void FinishReload();
	float CalculateDamage(bool& bOutIsCritical);
	void ApplyElementalEffect(AActor* Target);
};
