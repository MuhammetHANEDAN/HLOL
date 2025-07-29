// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_MagicalDamage.h"

#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"

UExecCalc_MagicalDamage::UExecCalc_MagicalDamage()
{
	
	DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, MagicalArmor, Target, false);
	DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, MagicalArmorPenetration, Source, false);

	RelevantAttributesToCapture.Add(MagicalArmorDef);
	RelevantAttributesToCapture.Add(MagicalArmorPenetrationDef);
}

void UExecCalc_MagicalDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	const FHGamePlayTags& Tags = FHGamePlayTags::Get();
	
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	
	float DamageTypeValue = Spec.GetSetByCallerMagnitude(Tags.Damage_Magical, false);
	
	float MagicalArmorValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicalArmorDef, EvaluationParameters, MagicalArmorValue);
	MagicalArmorValue = FMath::Clamp(MagicalArmorValue, 0.f, 1000.f);

	float MagicalArmorPenetrationValue = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicalArmorPenetrationDef,EvaluationParameters,MagicalArmorPenetrationValue);
	MagicalArmorPenetrationValue = FMath::Clamp(MagicalArmorPenetrationValue,0,1000.f);

	float EffectiveArmor = MagicalArmorValue * (1 - (MagicalArmorPenetrationValue/100));
	DamageTypeValue = DamageTypeValue * (100 / (100 + EffectiveArmor));
		
	Damage += DamageTypeValue;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UHAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
