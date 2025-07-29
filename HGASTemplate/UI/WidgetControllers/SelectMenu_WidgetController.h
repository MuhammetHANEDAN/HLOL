// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HGASTemplate/UI/WidgetControllers/HWidgetController.h"
#include "SelectMenu_WidgetController.generated.h"

class UHGameInstance;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API USelectMenu_WidgetController : public UHWidgetController
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UHGameInstance> HGameInstance;
	
	UHGameInstance* GetHGameInstance() const;

	UFUNCTION(BlueprintCallable)
	void SetTeam(bool bIsRadiant);

	UFUNCTION(BlueprintCallable)
	void SetHero(FGameplayTag HeroTag);
	
	
};
