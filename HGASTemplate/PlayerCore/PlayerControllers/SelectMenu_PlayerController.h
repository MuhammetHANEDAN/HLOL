// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "SelectMenu_PlayerController.generated.h"

/**
 * Select Menunun widget controlleri direk bu olacak
 */

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API ASelectMenu_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ASelectMenu_PlayerController();

protected:

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void SetIsRadiantPlayer(bool bIsRadiant);

	UFUNCTION(Server,Reliable,BlueprintCallable)
	void Server_SetIsRadiantPlayer(bool bIsRadiant);

	UFUNCTION(Server,Reliable,BlueprintCallable)
	void Server_SetHeroTag(FGameplayTag HeroTag);
	
};
