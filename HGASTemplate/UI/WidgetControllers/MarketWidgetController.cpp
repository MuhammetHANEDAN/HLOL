// Fill out your copyright notice in the Description page of Project Settings.


#include "MarketWidgetController.h"

#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"

void UMarketWidgetController::DebugMsg()
{
	UE_LOG(LogTemp, Display, TEXT("DebugMsg"));
}

void UMarketWidgetController::RequestBuyItem(FGameplayTag ItemTag)
{
	GetHPS()->AddItem(ItemTag);
}
