// Fill out your copyright notice in the Description page of Project Settings.


#include "Data_AbilityInfo.h"

#include "HGASTemplate/GamePlayTags/HGameplayTags.h"

FAbilityInfo* UData_AbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag) 
{
	if(AbilityInformation.Num() == 0) return nullptr;

	for (auto& Element : AbilityInformation)
	{
		if(Element.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return &Element;
		}
	}
	return nullptr;
}

