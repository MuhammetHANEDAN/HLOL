// Fill out your copyright notice in the Description page of Project Settings.


#include "Data_Hero.h"

FHeroDataStruct& UData_Hero::GetHeroDataByHeroTag(FGameplayTag HeroTag)
{
	if(HeroDatas.Num() == 0) return NullS;
	for (auto& Element : HeroDatas)
	{
		if(Element.HeroTag.MatchesTagExact(HeroTag))
		{
			return Element;
		}
	}
	return NullS;
}

UTexture2D* UData_Hero::GetHeroIconByHeroTag(FGameplayTag HeroTag)
{
	FHeroDataStruct Data = GetHeroDataByHeroTag(HeroTag);
	return Data.HeroIcon;
}
