// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionBarComponent.h"

#include "HGASTemplate/UI/Widget/HUserWidget.h"
#include "HGASTemplate/UI/WidgetControllers/HWidgetController.h"
#include "HGASTemplate/UI/WidgetControllers/MinionBarController.h"


// Sets default values for this component's properties
UMinionBarComponent::UMinionBarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMinionBarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMinionBarComponent::InitMinionBarComp(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	UHUserWidget* HWidget = Cast<UHUserWidget>(GetWidget());
	check(HWidget);

	check(ASC);
	check(AS);

	FWidgetControllerParams Params;
	Params.AttributeSet = AS;
	Params.AbilitySystemComponent = ASC;
	

	check(MinionBarControllerClass);
	MinionBarWidgetController = NewObject<UMinionBarController>(this,MinionBarControllerClass);

	MinionBarWidgetController->SetWidgetControllerParams(Params);

	HWidget->SetWidgetController(MinionBarWidgetController);

	MinionBarWidgetController->BindCallbacksToDependencies();
	
	
}




