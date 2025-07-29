// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicSkillInfoObject.h"

#include "HGASTemplate/AbilitySystem/HAttributeSet.h"

UDynamicSkillInfoObject::UDynamicSkillInfoObject()
{
	
}

float UDynamicSkillInfoObject::GetValueFromScalableFloat(const FScalableFloat ScalableFloat, int32 Level) const
{
	return ScalableFloat.GetValueAtLevel(Level);
}


