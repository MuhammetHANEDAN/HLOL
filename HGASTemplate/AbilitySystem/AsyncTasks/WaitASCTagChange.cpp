// Fill out your copyright notice in the Description page of Project Settings.


#include "WaitASCTagChange.h"

#include "AbilitySystemComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"

UWaitASCTagChange* UWaitASCTagChange::WaitAscTagChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                       const FGameplayTag& InTag)
{
	UWaitASCTagChange* WaitASCTagChange = NewObject<UWaitASCTagChange>();
	WaitASCTagChange->ASC = AbilitySystemComponent;
	WaitASCTagChange->Tag = InTag;

	if (!IsValid(AbilitySystemComponent) || !InTag.IsValid())
	{
		WaitASCTagChange->EndTask();
		return nullptr;
	}
	
	AbilitySystemComponent->RegisterGameplayTagEvent(
		InTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(
			WaitASCTagChange,
			&UWaitASCTagChange::ASCTagChanged);

	return WaitASCTagChange;
}

void UWaitASCTagChange::EndTask()
{
	if (!IsValid(ASC)) return;
	ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitASCTagChange::ASCTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		TagRemoved.Broadcast();
	}
	else if (NewCount > 0)
	{
		TagAdded.Broadcast();
	}
	
}

