// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalAttackAbility.h"
#include "NormalAttackRangedAbility.generated.h"

class AHProjectile;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UNormalAttackRangedAbility : public UNormalAttackAbility
{
	GENERATED_BODY()

public:
	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "AProps")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AProps")
	void SpawnProjectileHoming(AActor* HomingTargetActor,const FVector& SocketLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AProps")
	TSubclassOf<AHProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category= "AProps")
	float HomingAcceleration = 3200.f;

	
	
};
