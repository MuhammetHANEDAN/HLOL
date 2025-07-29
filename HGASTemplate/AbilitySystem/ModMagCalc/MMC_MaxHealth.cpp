
#include "MMC_MaxHealth.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"


UMMC_MaxHealth::UMMC_MaxHealth()
{
	/*VigorDef.AttributeToCapture = UHAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // Vigor nereden capturelendi.
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);*/
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Target and Source
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluationParameters,Vigor);
	Vigor = FMath::Max<float>(Vigor,0.f); // Min 0 olabilir eğer 0 dan küçükse 0 a eşitleyecek

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 80 + 2.5f*Vigor + 10.f*PlayerLevel;
	
	
}
