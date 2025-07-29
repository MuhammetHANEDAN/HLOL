// Fill out your copyright notice in the Description page of Project Settings.


#include "BarComponent.h"

#include "HGASTemplate/Characters/HBaseCharacter.h"
#include "HGASTemplate/PlayerCore/PlayerControllers/HPlayerController.h"
#include "HGASTemplate/UI/Widget/HUserWidget.h"
#include "HGASTemplate/UI/WidgetControllers/HWidgetController.h"
#include "HGASTemplate/UI/WidgetControllers/OverlayWidgetController.h"

void UBarComponent::BeginPlay()
{
	Super::BeginPlay();

	/*FTimerHandle TH;
	GetWorld()->GetTimerManager().SetTimer(TH,this,&ThisClass::Delayed_Works,5);*/
	
}

void UBarComponent::Delayed_Works()
{
	AHBaseCharacter* HBaseCharacter = Cast<AHBaseCharacter>(GetOwner());
	check(HBaseCharacter);
	
	UHUserWidget* HUserWidget = Cast<UHUserWidget>(GetWidget());
	if(HUserWidget == nullptr) return;
	if(HBaseCharacter->Controller && HBaseCharacter->Controller->IsLocalController() && HBaseCharacter->ActorHasTag(FName("Player"))) return;

	FWidgetControllerParams Params;
	Params.AttributeSet = HBaseCharacter->AttributeSet;
	if(HBaseCharacter->Controller)
	{
		Params.PlayerController = Cast<AHPlayerController>(HBaseCharacter->Controller);
	}
	
	Params.PlayerState = HBaseCharacter->GetPlayerState();
	
	Params.AbilitySystemComponent = HBaseCharacter->AbilitySystemComponent;

	check(WidgetControllerClass);
	WidgetController = NewObject<UOverlayWidgetController>(this,WidgetControllerClass);

	WidgetController->SetWidgetControllerParams(Params);
	HUserWidget->SetWidgetController(WidgetController);

	WidgetController->BindCallbacksToDependencies();
	
}

void UBarComponent::StartInitPlayerBarComp()
{
	FTimerHandle TH;
	GetWorld()->GetTimerManager().SetTimer(TH,this,&ThisClass::InitPlayerBarComp,1.2);
}

void UBarComponent::InitPlayerBarComp()
{
	AHBaseCharacter* HBaseCharacter = Cast<AHBaseCharacter>(GetOwner());
	check(HBaseCharacter);
	
	UHUserWidget* HUserWidget = Cast<UHUserWidget>(GetWidget());
	if(HUserWidget == nullptr) return;

	FWidgetControllerParams Params;
	Params.AttributeSet = HBaseCharacter->AttributeSet;
	if(HBaseCharacter->Controller)
	{
		Params.PlayerController = Cast<AHPlayerController>(HBaseCharacter->Controller);
	}
	
	Params.PlayerState = HBaseCharacter->GetPlayerState();
	
	Params.AbilitySystemComponent = HBaseCharacter->AbilitySystemComponent;

	check(WidgetControllerClass);
	WidgetController = NewObject<UOverlayWidgetController>(this,WidgetControllerClass);

	WidgetController->SetWidgetControllerParams(Params);
	HUserWidget->SetWidgetController(WidgetController);

	WidgetController->BindCallbacksToDependencies();

	if(HBaseCharacter->Controller && HBaseCharacter->Controller->IsLocalController() )
	{
		SetVisibility(false);
	}
	
}
