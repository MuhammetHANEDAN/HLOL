
#include "ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/Interfaces/HAbilitySystemInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

struct HDamageStatics
{

	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LifeSteal);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BashChance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalArmor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmor);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalArmorPenetration);
	
	HDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, LifeSteal, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, BashChance, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, PhysicalArmor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, MagicalArmor, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, PhysicalArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHAttributeSet, MagicalArmorPenetration, Source, false);
	}
};

static const HDamageStatics& DamageStatics()
{
	static HDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LifeStealDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().MagicalArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BashChanceDef);
}



void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
	const FHGamePlayTags& Tags = FHGamePlayTags::Get();

	TMap<FGameplayTag, FGameplayTag> DamageTypesToArmors;

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCapturePenetrationDefs;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToAttributes;
	
	DamageTypesToArmors.Add(Tags.Damage_Physical, Tags.Attributes_PhysicalArmor);
	DamageTypesToArmors.Add(Tags.Damage_Magical, Tags.Attributes_MagicalArmor);

	TagsToCaptureDefs.Add(Tags.Attributes_PhysicalArmor, DamageStatics().PhysicalArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_MagicalArmor, DamageStatics().MagicalArmorDef);

	TagsToCapturePenetrationDefs.Add(Tags.Attributes_PhysicalArmor,DamageStatics().PhysicalArmorPenetrationDef);
	TagsToCapturePenetrationDefs.Add(Tags.Attributes_MagicalArmor,DamageStatics().MagicalArmorPenetrationDef);

	DamageTypesToAttributes.Add(Tags.Damage_Physical, Tags.Attributes_AttackDamage);
	DamageTypesToAttributes.Add(Tags.Damage_Magical, Tags.Attributes_AbilityPower);
	
	
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayTagContainer EffectTags;
	Spec.GetAllAssetTags(EffectTags);
	
	// Debuffa ekstra bir müdahaale olmadığı için kullanmadık.
	//DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);

	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair  : DamageTypesToArmors)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ArmorTypeTag = Pair.Value;

		//checkf(TagsToCaptureDefs.Contains(ArmorTypeTag), TEXT("TagsToCaptureDefs doesn't contain Tag: [%s] in ExecCalc_Damage"), *ArmorTypeTag.ToString());

		// Eğer bu zırh tipi için bir tanımlama yoksa, uyarı ver ve devam et
		if (!TagsToCaptureDefs.Contains(ArmorTypeTag))
		{
			UE_LOG(LogTemp, Warning, TEXT("TagsToCaptureDefs haritasında %s etiketi için tanımlama bulunamadı!"), *ArmorTypeTag.ToString());
			continue;
		}
		
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false);
		
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}

		/**
		 * Gelen Damage Skillden gelmiş ise
		 * Mesel Fiziksel vuran bir skill, ve biz bu skilin damagesini karakterin fiziksel damage attributesine göre scale ediyoruz
		 * Normal Attack ise işin içine katılmıyorlar
		 */
		if (EffectTags.HasTagExact(Tags.GEffect_Skill)) 
		{
			if (SourceASC->Implements<UHAbilitySystemInterface>())
			{
				float Add = IHAbilitySystemInterface::Execute_I_GetAttributeValueByTag(SourceASC,DamageTypesToAttributes[DamageTypeTag]);
				UE_LOG(LogTemp,Error,TEXT("Add = %f"), Add);
				DamageTypeValue += Add;
			}
			
		}
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ArmorTypeTag];
		const FGameplayEffectAttributeCaptureDefinition CapturePenetrationDef = TagsToCapturePenetrationDefs[ArmorTypeTag];
		
		float ArmorTypeValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, ArmorTypeValue);
		ArmorTypeValue = FMath::Clamp(ArmorTypeValue, 0.f, 1000.f);

		float ArmorPenetrationValue = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CapturePenetrationDef,EvaluationParameters,ArmorPenetrationValue);
		ArmorPenetrationValue = FMath::Clamp(ArmorPenetrationValue,0,1000.f);

		float EffectiveArmor = ArmorTypeValue * (1 - (ArmorPenetrationValue/100));
		DamageTypeValue = DamageTypeValue * (100 / (100 + EffectiveArmor));
		
		Damage += DamageTypeValue;
	}
	

	if (EffectTags.HasTagExact(Tags.NormalAttack)) // Bash Durumu, Critical Hit, LifeSteal Sadece Normal Attacklarda Değerlendirilir
	{
		float BashChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BashChanceDef,EvaluationParameters,BashChance);
		if (BashChance > 0.f)
		{
			int32 Sel = UKismetMathLibrary::RandomIntegerInRange(0,100);
			if (Sel <= BashChance)
			{
				UHBlueprintFunctionLibrary::SetIsBashHit(EffectContextHandle,true);
			}
		}

		float CriticalHitChance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, CriticalHitChance);
		if (CriticalHitChance > 0.f)
		{
			int32 Sel = UKismetMathLibrary::RandomIntegerInRange(0,100);
			if (Sel <= CriticalHitChance)
			{
				UHBlueprintFunctionLibrary::SetIsCriticalHit(EffectContextHandle,true);
				Damage *= 2.f;
			}
		}

		float LifeSteal = 0.f;
		float LastLifeSteal = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LifeStealDef, EvaluationParameters, LifeSteal);
		if (LifeSteal > 0.f)
		{
			LastLifeSteal = Damage * (LifeSteal / 100);
			const FGameplayModifierEvaluatedData LifeStealData(UHAttributeSet::GetIncomingLifeStealAttribute(), EGameplayModOp::Additive,LastLifeSteal );
			OutExecutionOutput.AddOutputModifier(LifeStealData);
		}

		/*if (SourceASC && SourceAvatar && SourceAvatar->ActorHasTag(FName("Player")))
		{
			//SourceASC->TryActivateAbilitiesByTag(Tags.Item_Mjolnir.GetSingleTagContainer());
			for (auto& element : SourceASC->GetActivatableAbilities())
			{
				if (element.Ability->AbilityTags.HasTagExact(Tags.Item_Mjolnir) )
				{
					SourceASC->TryActivateAbility(element.Handle);
					break;
				}
			}
		}*/

		FGameplayEventData Payload;
		Payload.Target = TargetAvatar;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceASC->GetAvatarActor(), Tags.Item_Mjolnir, Payload);
		
	}
	
	//SourceTags->HasTagExact()
	
	const FGameplayModifierEvaluatedData EvaluatedData(UHAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FHGamePlayTags& Tags = FHGamePlayTags::Get();
	
	/*
	for (TTuple<FGameplayTag, FGameplayTag> Pair : Tags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f) // .5 padding for floating point [im]precision
		{
			// Determine if there was a successful debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(Tags.Debuff_Chance, false, -1.f);

			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = Tags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * ( 100 - TargetDebuffResistance ) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

				UHBlueprintFunctionLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UHBlueprintFunctionLibrary::SetDamageType(ContextHandle, DamageType);

				const float DebuffDamage = Spec.GetSetByCallerMagnitude(Tags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(Tags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(Tags.Debuff_Frequency, false, -1.f);

				UHBlueprintFunctionLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UHBlueprintFunctionLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UHBlueprintFunctionLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}*/
}
