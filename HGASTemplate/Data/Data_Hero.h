// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Data_Hero.generated.h"

USTRUCT(BlueprintType)
struct FHeroDataStruct
{
	GENERATED_BODY()

	FHeroDataStruct(){}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag HeroTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> HeroIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> PassiveIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> SkillQIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> SkillWIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> SkillEIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> SkillRIcon = nullptr;
	
};

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UData_Hero : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FHeroDataStruct> HeroDatas;

	FHeroDataStruct NullS;
	
	FHeroDataStruct& GetHeroDataByHeroTag(FGameplayTag HeroTag);

	UTexture2D* GetHeroIconByHeroTag(FGameplayTag HeroTag);
	
};
