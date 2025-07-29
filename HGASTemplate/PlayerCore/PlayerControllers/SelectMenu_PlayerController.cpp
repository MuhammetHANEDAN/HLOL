// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMenu_PlayerController.h"

#include "HGASTemplate/GameCore/HGameInstance.h"
#include "HGASTemplate/GameCore/GameModes/SelectMenuGameMode.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"

ASelectMenu_PlayerController::ASelectMenu_PlayerController()
{
	bReplicates = true;
}

void ASelectMenu_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASelectMenu_PlayerController::SetIsRadiantPlayer(bool bIsRadiant)
{
	if(UHGameInstance* HGameInstacne = GetGameInstance<UHGameInstance>())
	{
		HGameInstacne->SetIsRadiantPlayer(bIsRadiant);
	}
	
}

void ASelectMenu_PlayerController::Server_SetHeroTag_Implementation(FGameplayTag HeroTag)
{
	AHPlayerState* PS = GetPlayerState<AHPlayerState>();
	check(PS);

	PS->Server_SetHeroTag(HeroTag);
}

void ASelectMenu_PlayerController::Server_SetIsRadiantPlayer_Implementation(bool bIsRadiant)
{
	
}
