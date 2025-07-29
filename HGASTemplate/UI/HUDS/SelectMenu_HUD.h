// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SelectMenu_HUD.generated.h"

class USelectMenu_WidgetController;
class USelectMenuWidget;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API ASelectMenu_HUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TSubclassOf<USelectMenuWidget> SelectMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TSubclassOf<USelectMenu_WidgetController> SelectMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USelectMenuWidget> SelectMenuWidget;
	
};
