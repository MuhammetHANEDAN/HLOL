// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMenuWidget.generated.h"

class USelectMenu_WidgetController;
class ASelectMenu_PlayerController;
/**
 * Widget oluşturulunca SelectMenuController ın setlenmesi gerek setleyince WidgetControllerSet çağır
 */
UCLASS()
class HGASTEMPLATE_API USelectMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

	UFUNCTION(BlueprintCallable)
	void SetSelectMenuController(USelectMenu_WidgetController* InSelectMenuWidgetController);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<USelectMenu_WidgetController> SelectMenuController;
	
};
