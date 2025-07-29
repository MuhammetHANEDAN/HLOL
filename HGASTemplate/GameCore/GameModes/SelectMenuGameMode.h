// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SelectMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API ASelectMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASelectMenuGameMode();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	/** Geçici Çözüm SelectMenu işlemleri için ilerde değiştir*/

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float TimeToTravel = 15.f;
	float TrackTimeToTravel = 0;
	bool bTravelling = false;

	UPROPERTY(EditDefaultsOnly)
	FString Path = FString();
	
	void TravelGameMap();

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	TSoftObjectPtr<UWorld> TravelWorld;
	
};
