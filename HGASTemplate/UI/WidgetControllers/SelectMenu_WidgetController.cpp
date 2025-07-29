// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMenu_WidgetController.h"

#include "GameFramework/PlayerState.h"
#include "HGASTemplate/GameCore/HGameInstance.h"
#include "HGASTemplate/PlayerCore/PlayerControllers/HPlayerController.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"

UHGameInstance* USelectMenu_WidgetController::GetHGameInstance() const
{
	if(HGameInstance == nullptr)
	{
		check(PlayerController);
		return PlayerController->GetGameInstance<UHGameInstance>();
	}
	return HGameInstance;
}

void USelectMenu_WidgetController::SetTeam(bool bIsRadiant)
{
	if(GetHGameInstance())
	{
		GetHGameInstance()->bIsRadiantPlayer = bIsRadiant;
	}
	
	/*AHPlayerState* PS = PlayerController->GetPlayerState<AHPlayerState>();
	if (!PS) return;
    
	PS->Server_SetIsRadiantPlayer(bIsRadiant);*/

	/*AHPlayerController* PC = Cast<AHPlayerController>(PlayerController);
	PC->Server_SetIsRadiant_Implementation(bIsRadiant);*/

}

void USelectMenu_WidgetController::SetHero(FGameplayTag HeroTag)
{
	/*AHPlayerState* PS = PlayerController->GetPlayerState<AHPlayerState>();
	if (!PS) return;

	PS->Server_SetHeroTag(HeroTag);*/

	if(GetHGameInstance())
	{
		GetHGameInstance()->HeroTag = HeroTag;
	}

	/*AHPlayerController* PC = Cast<AHPlayerController>(PlayerController);
	PC->Server_SetHeroTag(HeroTag);*/
	
}
