// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Data_AbilityInfo.generated.h"

/**
 * 
 */

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAbilityLevel = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

UCLASS()
class HGASTEMPLATE_API UData_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAbilityInfo> AbilityInformation;

	/** R skilinin hangi levelde maksimum kaç olabileceği bilgisidir*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TMap<int32, int32> RAbilityMaxLevels;

	FAbilityInfo* FindAbilityInfoForTag(const FGameplayTag& AbilityTag) ;
	
	
};

