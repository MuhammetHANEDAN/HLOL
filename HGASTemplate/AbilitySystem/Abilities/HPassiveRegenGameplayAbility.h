// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HPassiveGameplayAbility.h"
#include "HPassiveRegenGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHPassiveRegenGameplayAbility : public UHPassiveGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UAbilitySystemComponent* ASC;

	UPROPERTY(BlueprintReadWrite, Category = "AProps")
	float HPRegen = 0;
	UPROPERTY(BlueprintReadWrite, Category = "AProps")
	float MPRegen = 0;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AProps")
	TSubclassOf<UGameplayEffect> RegenEffectClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
