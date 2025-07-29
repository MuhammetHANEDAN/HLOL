// Fill out your copyright notice in the Description page of Project Settings.


#include "HSkillToolTip.h"

#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"

UHGameplayAbility* UHSkillToolTip::GetHAbilityFromTag(const FGameplayTag AbilityTag)
{
	if (HASC.IsValid())
	{
		return HASC->GetHAbilityFromTag(AbilityTag);
	}
	return nullptr;
}

FString UHSkillToolTip::GetSkillDescription(const FGameplayTag& SkillTag)
{
	if (HASC.IsValid())
	{
		return HASC->GetSkillDescriptionByTag(SkillTag);
	}
	return FString();
}

FDynamicSkillInfo UHSkillToolTip::GetDynamicSkillInfo(const FGameplayTag& SkillTag)
{
	if (HASC.IsValid())
	{
		return HASC->GetDynamicSkillInfoByTag(SkillTag);
	}
	return FDynamicSkillInfo();
}


