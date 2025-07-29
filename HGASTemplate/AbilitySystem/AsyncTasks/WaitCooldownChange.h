// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * Editörden BlueprintAsyncActionBase yazarak oluşturabilirsin
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class HGASTEMPLATE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();
	
protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
