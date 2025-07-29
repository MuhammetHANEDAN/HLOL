// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "HGASTemplate/AbilitySystem/HAbilityTypes/HAbilityTypes.h"
#include "HGameplayAbility.generated.h"

class UDynamicSkillInfoObject;
class UHAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	/*UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AProps")
	TObjectPtr<UAbilitySystemComponent> CachedAbilitySystemComponent;*/ //?

	UHGameplayAbility();
	
	UPROPERTY(EditDefaultsOnly, Category="AProps")
	FGameplayTag SkillInputTag;

	UFUNCTION(BlueprintPure)
	float GetManaCost(float InLevel = 1.f) const;
	UFUNCTION(BlueprintPure)
	float GetCooldown(float InLevel = 1.f) const;
	
	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle H_ApplySpecToOwner(const TSubclassOf<UGameplayEffect> EffectClass, int32 Level);

	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyNonReplicatedEffectToOwner(const FGameplayTag AttributeTag,EGameplayEffectDurationType DurationType, float Frequency = 0,
	float Duration = 0, float ValueToAdd = 0);
	
	UFUNCTION(BlueprintPure)
	FGameplayEffectContextHandle MakeContextHandleWithSourceActor(AActor* SourceActor);

	UFUNCTION(BlueprintPure)
	float GetValueFromScalableFloat(const FScalableFloat ScalableFloat, int32 Level) const;
	
	/**
	 * ------------------------------
	 */
	
	/**
	 * Önemli Sadece instanced olan GA larda kullan. GA başlatılmadan kullanma, çünkü world = nulldır başlatmadan
	 */
	UFUNCTION(BlueprintCallable)
	float GetAttributeValueByTag(const FGameplayTag AttributeTag) const;

	/** Verdiğin Targetin Locationunu değiştirmez. Targeting Locationu alır Offsetler ve Locationu döndürür*/
	UFUNCTION(BlueprintCallable)
	FVector OffsetActorLocation(const AActor* TargetActor, float XOffset = 0.f, float ZOffset = 0.f ) const ;

	
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "AProps")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	FGameplayTag DamageTypeTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	bool bIsDebuff = false;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	FGameplayTag DebuffTypeTag;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float DeathImpulseMagnitude = 1000.f;

	// EffectContext dışındada aşağıdakiler kullanılabilir. BP den direk ekstra GE uygulama amaçlı

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	bool bIsSlow = false;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	float SlowPercent = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	bool bIsKnockBack = false;

	/**
	 * Dynamic Info For Tooltip
	 */

	UPROPERTY(EditDefaultsOnly, Category = "AProps")
	TSubclassOf<UDynamicSkillInfoObject> DynamicSkillInfoObjectClass;
	
};
