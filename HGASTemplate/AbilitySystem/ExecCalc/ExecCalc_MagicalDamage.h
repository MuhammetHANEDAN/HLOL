﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_MagicalDamage.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UExecCalc_MagicalDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	
	UExecCalc_MagicalDamage();
	
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

protected:

	//FGameplayEffectAttributeCaptureDefinition MagicalArmorDef;
	//FGameplayEffectAttributeCaptureDefinition MagicalArmorPenetrationDef;

	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmorPenetration);

	
};
