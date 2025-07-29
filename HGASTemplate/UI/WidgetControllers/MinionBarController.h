// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HWidgetController.h"
#include "MinionBarController.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UMinionBarController : public UHWidgetController
{
	GENERATED_BODY()

	public:

	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable)
	FOnFloatStatChangedSignature OnHealthChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnFloatStatChangedSignature OnMaxHealthChangedDelegate;

	
};
