
#include "HWidgetController.h"

#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/PlayerCore/PlayerControllers/HPlayerController.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"


void UHWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UHWidgetController::BroadcastInitialValues()
{
	
}

void UHWidgetController::BindCallbacksToDependencies()
{
	
}

void UHWidgetController::BroadcastAbilityInfo()
{
	if (!GetHASC()->bStartupAbilitiesGiven) return;
	/*
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FHAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(HAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = HAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = HAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetHASC()->ForEachAbility(BroadcastDelegate);*/
}

AHPlayerController* UHWidgetController::GetHPC()
{
	if (HPlayerController == nullptr)
	{
		HPlayerController = Cast<AHPlayerController>(PlayerController);
	}
	return HPlayerController;
}

AHPlayerState* UHWidgetController::GetHPS()
{
	if (HPlayerState == nullptr)
	{
		HPlayerState = Cast<AHPlayerState>(PlayerState);
	}
	return HPlayerState;
}
UHAbilitySystemComponent* UHWidgetController::GetHASC()
{
	if (HAbilitySystemComponent == nullptr)
	{
		HAbilitySystemComponent = Cast<UHAbilitySystemComponent>(AbilitySystemComponent);
	}
	return HAbilitySystemComponent;
}
UHAttributeSet* UHWidgetController::GetHAS()
{
	if (HAttributeSet == nullptr)
	{
		HAttributeSet = Cast<UHAttributeSet>(AttributeSet);
	}
	return HAttributeSet;
}
/*
AHBaseCharacter* UHWidgetController::GetHBaseCharacter()
{
	if(AbilitySystemComponent)
	{
		if(AHBaseCharacter* HBC = Cast<AHBaseCharacter>(AbilitySystemComponent->GetAvatarActor()))
		{
			return HBC;
		}
		return nullptr;
	}
	return nullptr;
}
*/