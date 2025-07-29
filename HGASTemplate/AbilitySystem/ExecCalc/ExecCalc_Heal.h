// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Heal.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UExecCalc_Heal : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	
	UExecCalc_Heal();
	

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
