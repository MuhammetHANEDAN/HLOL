// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMenuWidget.h"

class USelectMenu_WidgetController;

void USelectMenuWidget::SetSelectMenuController(USelectMenu_WidgetController* InSelectMenuWidgetController )
{
	SelectMenuController = InSelectMenuWidgetController;
	WidgetControllerSet();
}
