// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HWidgetController.h"
#include "MarketWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UMarketWidgetController : public UHWidgetController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void DebugMsg();

	UFUNCTION(BlueprintCallable)
	void RequestBuyItem(FGameplayTag ItemTag);

	
};
