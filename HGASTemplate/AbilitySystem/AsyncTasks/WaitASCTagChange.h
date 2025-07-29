// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitASCTagChange.generated.h"

/**
 * 
 */

class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTagAddedOrRemoewdSignature);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class HGASTEMPLATE_API UWaitASCTagChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FTagAddedOrRemoewdSignature TagAdded;

	UPROPERTY(BlueprintAssignable)
	FTagAddedOrRemoewdSignature TagRemoved;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitASCTagChange* WaitAscTagChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag Tag;

	void ASCTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	
	
};
