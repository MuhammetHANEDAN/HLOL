// Fill out your copyright notice in the Description page of Project Settings.


#include "Data_Item.h"

FItemDataStruct& UData_Item::GetItemDataFromTag(FGameplayTag ItemTag)
{
	if(ItemDatas.Num() == 0) return NullS;
	for (auto& Element : ItemDatas)
	{
		if(Element.ItemTag.MatchesTagExact(ItemTag))
		{
			return Element;
		}
	}
	return NullS;
}
