// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAttackAbility.h"

AActor* UNormalAttackAbility::GetTargetActor() const
{
	if (!TargetActor.IsValid())
	{
		return nullptr;
	}
	return TargetActor.Get();
}
