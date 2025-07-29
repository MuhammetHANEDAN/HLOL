// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionBarController.h"

#include "AbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"

void UMinionBarController::BindCallbacksToDependencies()
{
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHAS()->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnHealthChangedDelegate.Broadcast(Data.NewValue);
	});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHAS()->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
	{
		OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
	});
	
	
}

void UMinionBarController::BroadcastInitialValues()
{

	//OnHealthChangedDelegate.Broadcast(GetHAS()->GetHealth());
	//OnMaxHealthChangedDelegate.Broadcast(GetHAS()->GetMaxHealth());

	OnHealthChangedDelegate.Broadcast(GetHAS()->GetHealthAttribute().GetNumericValue(GetHAS()));
	OnMaxHealthChangedDelegate.Broadcast(GetHAS()->GetMaxHealthAttribute().GetNumericValue(GetHAS()));
	
}
