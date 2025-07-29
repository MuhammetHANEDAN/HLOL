// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMenuGameMode.h"

#include "Kismet/GameplayStatics.h"

ASelectMenuGameMode::ASelectMenuGameMode()
{
	bUseSeamlessTravel = true;
	
	
}

void ASelectMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&ASelectMenuGameMode::TravelGameMap,TimeToTravel);
	
}

void ASelectMenuGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

void ASelectMenuGameMode::TravelGameMap()
{
	UE_LOG(LogTemp,Error,TEXT("Travelling Started"));
	bTravelling = true;
	GetWorld()->ServerTravel(Path);
}
