// Fill out your copyright notice in the Description page of Project Settings.


#include "HGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AudioMixerBlueprintLibrary.h"
#include "Animation/AnimNode_Inertialization.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"

UHGameplayAbility::UHGameplayAbility()
{
	
}

float UHGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UHAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UHGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}

FActiveGameplayEffectHandle UHGameplayAbility::H_ApplySpecToOwner(const TSubclassOf<UGameplayEffect> EffectClass,
	int32 Level)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	AActor* OwnerC = GetAvatarActorFromActorInfo();

	check(ASC);
	check(OwnerC);

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(OwnerC);
	ContextHandle.AddInstigator(OwnerC,OwnerC);

	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
	
	//return ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get(),GetCurrentActivationInfo().GetActivationPredictionKey()); Alt Satıra geçilebilir kontrol et bug oluştumu
	// Predict key verince sadece serverden uygulanan infinity ve duration based GE lerde sıkıntı oluyordu
	return ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

FActiveGameplayEffectHandle UHGameplayAbility::ApplyNonReplicatedEffectToOwner(const FGameplayTag AttributeTag, EGameplayEffectDurationType DurationType,
	float Frequency,float Duration,float ValueToAdd)
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	//const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();
	FGameplayEffectContextHandle EffectContext = ASC ->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());
	
	FString Name = FString::Printf(TEXT("Attribute _%s"), *AttributeTag.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(Name));
	
	Effect->DurationPolicy = DurationType;
	Effect->Period = Frequency;
	Effect->DurationMagnitude = FScalableFloat(Duration);
	
	/* Belki etkinleştirilebilir
	FInheritedTagContainer TagContainer;
	TagContainer.AddTag(DebuffTypeTag);
	auto& Comp = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	Comp.SetAndApplyTargetTagChanges(TagContainer);*/
	
	Effect->StackingType = EGameplayEffectStackingType::None;
	//Effect->StackLimitCount = 1;
	const int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	
	ModifierInfo.ModifierMagnitude = FScalableFloat(ValueToAdd);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;

	const UHAttributeSet* AS = Cast<UHAttributeSet>(ASC->GetAttributeSet(UHAttributeSet::StaticClass()));
	check(AS);

	FGameplayAttribute TargetGameplayAttribute;
	bool GamePlayAttributeFound = false;
	
	for (auto& Element : AS->TagsToAttributes)
	{
		if (Element.Key == AttributeTag)
		{
			TargetGameplayAttribute = Element.Value();
			GamePlayAttributeFound = true;
		}
	}

	if (GamePlayAttributeFound == false) return FActiveGameplayEffectHandle();
	
	ModifierInfo.Attribute = TargetGameplayAttribute;

	FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f);

	return ASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
}

FGameplayEffectContextHandle UHGameplayAbility::MakeContextHandleWithSourceActor(AActor* SourceActor)
{

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	check(IsValid(ASC));
	
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(this); // important

	return ContextHandle;
}

float UHGameplayAbility::GetValueFromScalableFloat(const FScalableFloat ScalableFloat, int32 Level) const
{
	return ScalableFloat.GetValueAtLevel(Level);
}

float UHGameplayAbility::GetAttributeValueByTag(const FGameplayTag AttributeTag) const
{
	if (GetAbilitySystemComponentFromActorInfo() == nullptr)
	{
		return 0.f;
	}
	
	if (const UHAttributeSet* AttributeSet = Cast<UHAttributeSet>(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UHAttributeSet::StaticClass())))
	{
		for (auto& Element : AttributeSet->TagsToAttributes)
		{
			if (AttributeTag.MatchesTagExact(Element.Key))
			{
				return Element.Value().GetNumericValue(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UHAttributeSet::StaticClass()));
			}
		}
	}
	return 0.f;
}

FVector UHGameplayAbility::OffsetActorLocation(const AActor* TargetActor, float XOffset, float ZOffset) const
{
	if (IsValid(TargetActor))
	{
		const FVector CurrentLoc = TargetActor->GetActorLocation();
		const FVector Forward = TargetActor->GetActorForwardVector();
		const FVector Up = TargetActor->GetActorUpVector();

		const FVector N = TargetActor->GetActorLocation() + (Forward*XOffset);
		return N + (Up*ZOffset);
	}
	return FVector::ZeroVector;
}

/** Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel()); Kullanılarak Damage CT sinden çekilip setleniyor */
FDamageEffectParams UHGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,
	bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverridePitch,
	float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageTypeTag = DamageTypeTag;
	Params.bIsDebuff = bIsDebuff;
	Params.DebuffTypeTag = DebuffTypeTag;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.bIsSlow = bIsSlow;
	Params.SlowPercent = SlowPercent;
	Params.bIsKnockBack = bIsKnockBack;

	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if (!bOverrideKnockbackDirection)
		{
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
	}
	
	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		Params.KnockbackForce = KnockbackDirectionOverride * KnockbackForceMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}
	
	return Params;
}

float UHGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}







