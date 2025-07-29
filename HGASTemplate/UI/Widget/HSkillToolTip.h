// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "HSkillToolTip.generated.h"

struct FDynamicSkillInfo;
class UHGameplayAbility;
struct FGameplayAbilitySpec;
class UHAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHSkillToolTip : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,meta=(ExposeOnSpawn))
	TWeakObjectPtr<UHAbilitySystemComponent> HASC;

	UFUNCTION(BlueprintPure)
	UHGameplayAbility* GetHAbilityFromTag(const FGameplayTag AbilityTag);

	UFUNCTION(BlueprintPure)
	FString GetSkillDescription(const FGameplayTag& SkillTag);

	UFUNCTION(BlueprintPure)
	FDynamicSkillInfo GetDynamicSkillInfo(const FGameplayTag& SkillTag);
	
};
