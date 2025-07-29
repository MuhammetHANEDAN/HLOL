// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HDamageGameplayAbility.h"
#include "HProjectileAbility.generated.h"

class AHProjectile;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHProjectileAbility : public UHDamageGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileSpawnLocation, const FRotator& ProjectileSpawnRotation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 1;
	
};
