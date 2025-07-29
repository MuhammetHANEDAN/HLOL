// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "BarComponent.generated.h"

class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	
	UPROPERTY(EditDefaultsOnly, Category="AProps")
	TSubclassOf<UOverlayWidgetController> WidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> WidgetController;

	void Delayed_Works();

	void StartInitPlayerBarComp();

	void InitPlayerBarComp();
	
};
