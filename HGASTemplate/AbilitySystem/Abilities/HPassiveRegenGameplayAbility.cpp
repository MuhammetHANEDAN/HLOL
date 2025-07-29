// Fill out your copyright notice in the Description page of Project Settings.


#include "HPassiveRegenGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"

void UHPassiveRegenGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	check(GetAbilitySystemComponentFromActorInfo());
	check(GetAbilitySystemComponentFromActorInfo()->GetAttributeSet(UHAttributeSet::StaticClass()));

	ASC = GetAbilitySystemComponentFromActorInfo();
	const UHAttributeSet* AS = Cast<UHAttributeSet>(ASC->GetAttributeSet(UHAttributeSet::StaticClass()));
	
	FGameplayAttribute HPRegenAttribute = Cast<UHAttributeSet>(ASC->GetAttributeSet(UHAttributeSet::StaticClass()))->GetHealthRegenAttribute();
	FGameplayAttribute MPRegenAttribute = Cast<UHAttributeSet>(ASC->GetAttributeSet(UHAttributeSet::StaticClass()))->GetManaRegenAttribute();

	HPRegen = HPRegenAttribute.GetNumericValue(AS);
	MPRegen = MPRegenAttribute.GetNumericValue(AS);

	ASC->GetGameplayAttributeValueChangeDelegate(HPRegenAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			// Health Regen değeri değiştiğinde güncelleyin
			HPRegen = Data.NewValue;
		});

	ASC->GetGameplayAttributeValueChangeDelegate(MPRegenAttribute).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			MPRegen = Data.NewValue;
		});
	
}




