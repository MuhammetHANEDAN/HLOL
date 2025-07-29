// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HGameplayAbility.h"
#include "HGASTemplate/AbilitySystem/HAbilityTypes/HAbilityTypes.h"
#include "HDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHDamageGameplayAbility : public UHGameplayAbility
{
	GENERATED_BODY()

public:

	/**
	 *  Damage Works
	 */

	// İlerde Silinebilir
	UFUNCTION(BlueprintCallable) 
	void CauseDamage(AActor* TargetActor); 

	// İlerde Silinebilir
	UFUNCTION(BlueprintCallable)
	void DealDamage(const TSubclassOf<UGameplayEffect>& DamageGamePlayEffectClass, int32 Level,AActor* TargetActor );

	
	
	
	

	
};
