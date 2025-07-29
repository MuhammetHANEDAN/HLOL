// Fill out your copyright notice in the Description page of Project Settings.


#include "HAbilitySystemGlobals.h"

#include "HAbilityTypes/HAbilityTypes.h"

FGameplayEffectContext* UHAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FHGameplayEffectContext();
}
