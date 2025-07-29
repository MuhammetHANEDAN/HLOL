// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "HGASTemplate/Data/HDataStructTypes.h"
#include "UObject/Object.h"
#include "DynamicSkillInfoObject.generated.h"

class UAttributeSet;
class UHAttributeSet;
class UHAbilitySystemComponent;
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class HGASTEMPLATE_API UDynamicSkillInfoObject : public UObject
{
	GENERATED_BODY()

public:

	UDynamicSkillInfoObject();
	
	/** Kesinlikle Düzgün Seçmen Lazım*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Aprops")
	FGameplayTag SkillTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Aprops")
	FString SkillDescription;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentSkillLevel = 1;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSkillInfoStruct();

	/** UpdateSkill Desc gelince bu updatelenmeli*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Aprops")
	FDynamicSkillInfo DynamicSkillInfoStruct = FDynamicSkillInfo();

	UFUNCTION(BlueprintCallable)
	float GetValueFromScalableFloat(const FScalableFloat ScalableFloat, int32 Level) const;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Aprops")
	float AttackDamage = 0;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Aprops")
	float AbilityPower = 0;
	
	
};
