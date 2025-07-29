// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "HBlueprintFunctionLibrary.generated.h"


struct FScalableFloat;
class UHGameplayAbility;
class UData_Item;
class UMarketWidgetController;
class UGameplayEffect;
struct FDamageEffectParams;
class UOverlayWidgetController;
struct FWidgetControllerParams;
class AHHUD;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	 * Widget Controller
	 */

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AHHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure)
	UMarketWidgetController* MakeMarketWidgetController(UObject* OuterObject);

	/**
	 * Data
	 */

	UFUNCTION(BlueprintPure)
	static UData_Item* GetItemDataAsset(const UObject* ContextObj);

	
	
	
	/*
	 * Ability System Class Defaults
	 */

	/**
	 * Damage
	 */

	/**
	 * Düşmanlar arasında çalışır
	 * Daha çok damage uygulamak için kullan
	 * Pyhsical veya Magical Damage Tag koyarak - Exec Calc Damage ye göndermek için
	 */
	UFUNCTION(BlueprintCallable, Category= "HBFSystemLibrary|Damage")
	static void ApplyGamePlaySpecWithSetByCallerTagAndClass (TSubclassOf<UGameplayEffect> InGameplayEffectClass,
		const FGameplayTag InSetByCallerTag, float Magnitude,
	AActor* EffectCauserActor,AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|Damage")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category = "HBFSystemLibrary|Damage")
	static float GetAttributeValueByTag(UPARAM(ref)AActor* OwnerActor, const FGameplayTag AttributeTag);

	UFUNCTION(BlueprintPure, Category = "HBFSystemLibrary|Damage")
	static float GetValueFromScalableFloat(const FScalableFloat ScalableFloat, int32 Level);

	UFUNCTION(BlueprintPure, Category = "HBFSystemLibrary|UI")
	static UTexture2D* GetHeroIcon(APlayerController* PC, const FGameplayTag HeroTag);

	UFUNCTION(BlueprintPure, Category = "HBFSystemLibrary|Location")
	static FVector GetRecallLocation(const ACharacter* RecallCharacter, bool bIsRadiant, FRotator& OutRotator);

	/*
	 * Gameplay Mechanics
	 */

	UFUNCTION(BlueprintPure, Category = "HBFSystemLibrary|GameplayMechanics")
	static bool IsEnemy(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static void CancelAllActionAbilities(UPARAM(ref) UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static AActor* GetClosestTarget(UPARAM(ref)AActor* Actor, const TArray<AActor*>& ActorsToIgnore, float Radius);

	UFUNCTION(BlueprintPure, Category = "HBFSystemLibrary|GameplayMechanics")
	static FString GetFormatTimeAsText(const float TotalSeconds);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static AActor* Minion_GetClosestTarget(UPARAM(ref)AActor* Actor, const TArray<AActor*>& ActorsToIgnore, float Radius);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static AActor* Tower_GetClosestTarget(UPARAM(ref)AActor* Actor, const TArray<AActor*>& ActorsToIgnore, float Radius);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static void H_AddReplicatedLooseTagToASC(UPARAM(ref) UAbilitySystemComponent* ASC, FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static bool H_HaveASCAbilityWithThisTag(UPARAM(ref) UAbilitySystemComponent* ASC, FGameplayTag Tag);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetEnemyLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin,
	UPARAM(ref)AActor* OwnerActor);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetJungleLiveMobWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors,
		UPARAM(ref)AActor* ActorToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static AActor* GetTowerWithinRadius(const UObject* WorldContextObject,float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin);

	UFUNCTION(BlueprintCallable, Category = "HBFSystemLibrary|GameplayMechanics")
	static void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets,UPARAM(ref) AActor* OwnerActor, const FVector& TargetLoc);
	

	
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors);
	
	/*
	 * Effect Context Getters - Stters
	 */

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool GetIsDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuff);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDebuffTypeTag(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffTypeTag(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDebuffTypeTag);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageTypeTag(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageTypeTag);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool GetIsBashHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBashHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBashHit);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool GetIsSlow(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSlow(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSlow);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetSlowPercent(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetSlowPercent(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float bInIsSlowPercent);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool GetIsKnockBack(const FGameplayEffectContextHandle& EffectContextHandle);
	
	
	/*
	 * ?
	 */
	
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetKnockbackDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
	
};
