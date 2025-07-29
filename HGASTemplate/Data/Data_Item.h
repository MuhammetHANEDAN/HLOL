// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "HGASTemplate/AbilitySystem/Abilities/HGameplayAbility.h"
#include "Data_Item.generated.h"

USTRUCT(BlueprintType)
struct FItemDataStruct
{
	GENERATED_BODY()

	FItemDataStruct(){}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UHGameplayAbility> ItemAbilityClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BuyPrice = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SellPrice = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName = FText::FromString(TEXT("None"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemDescription = FText::FromString(TEXT(""));
	
};

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UData_Item : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FItemDataStruct> ItemDatas;

	FItemDataStruct NullS;

	UFUNCTION(BlueprintPure)
	FItemDataStruct& GetItemDataFromTag(FGameplayTag ItemTag);
	
};
