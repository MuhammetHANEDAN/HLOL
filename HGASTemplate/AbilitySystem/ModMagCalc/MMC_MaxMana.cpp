
#include "MMC_MaxMana.h"

#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"

class ICombatInterface;

UMMC_MaxMana::UMMC_MaxMana()
{
	/*IntelligenceDef.AttributeToCapture = UHAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target; // Intelligence nereden capturelendi.
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);*/
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Target and Source
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0;
	GetCapturedAttributeMagnitude(IntelligenceDef,Spec,EvaluationParameters,Intelligence);
	Intelligence = FMath::Max<float>(Intelligence,0.f); // Min 0 olabilir eğer 0 dan küçükse 0 a eşitleyecek
	
	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 50 + 2.5f *Intelligence + 15 * PlayerLevel;
}
