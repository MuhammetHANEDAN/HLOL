// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HDamageGameplayAbility.h"
#include "NormalAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UNormalAttackAbility : public UHDamageGameplayAbility
{
	GENERATED_BODY()

public:

	UNormalAttackAbility()
	{
		bCanMoveInterupted = false;
		bEndNormalAttackAbilityWhenAttackAnimEnd = false;
		TargetActor = nullptr;
	}


	UPROPERTY()
	TWeakObjectPtr<AActor>TargetActor;

	UFUNCTION(BlueprintCallable)
	AActor* GetTargetActor() const;

	/**
	 * Normal Attack GA nın başında false, Bitimine yakın True setle 
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite) 
	bool bCanMoveInterupted = false; // ?

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bEndNormalAttackAbilityWhenAttackAnimEnd = false; // ?

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AProps")
	float  NormalAttackSlowPercent = 0.f;
	
};
