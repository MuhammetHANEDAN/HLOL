// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMenu_HUD.h"

#include "HGASTemplate/PlayerCore/PlayerControllers/SelectMenu_PlayerController.h"
#include "HGASTemplate/UI/Widget/SelectMenuWidget.h"
#include "HGASTemplate/UI/WidgetControllers/SelectMenu_WidgetController.h"

void ASelectMenu_HUD::BeginPlay()
{
	Super::BeginPlay();

	check(SelectMenuWidgetClass);

	APlayerController* OwnerController = GetOwningPlayerController();
	check(OwnerController);
	
	SelectMenuWidget = CreateWidget<USelectMenuWidget>(OwnerController,SelectMenuWidgetClass);
	check(SelectMenuWidget);

	USelectMenu_WidgetController* SelectMenuWidgetController = NewObject<USelectMenu_WidgetController>(this,SelectMenuWidgetControllerClass);
	SelectMenuWidgetController->PlayerController = OwnerController;

	
	SelectMenuWidget->AddToViewport();
	SelectMenuWidget->SetSelectMenuController(SelectMenuWidgetController);
	SelectMenuWidget->WidgetControllerSet();
	
	FInputModeUIOnly UIOnly;
	OwnerController->SetInputMode(UIOnly);
	OwnerController->SetShowMouseCursor(true);
	
	
}
