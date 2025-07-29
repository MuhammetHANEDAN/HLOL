// Fill out your copyright notice in the Description page of Project Settings.


#include "HSpawner.h"

#include "Components/ArrowComponent.h"
#include "HGASTemplate/Interfaces/HGameModeInterface.h"
#include "GameFramework/GameModeBase.h"


// Sets default values
AHSpawner::AHSpawner()
{
	
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow Component");
	SetRootComponent(ArrowComponent);
}

// Called when the game starts or when spawned
void AHSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AHSpawner::GetGameTimeSeconds() const
{
	if (AGameModeBase* GM = GetWorld()->GetAuthGameMode())
	{
		IHGameModeInterface* IC = Cast<IHGameModeInterface>(GM);
		if (IC == nullptr) return 0.f;

		return IC->I_GetGameTime();
	}
	return 0.f;
}

int32 AHSpawner::CalculateLevelForTime(float TimeSeconds)
{
	int32 Level = 1;
	float Minute = TimeSeconds/60;

	if (Minute <= 1)
	{
		
	}
	else if (Minute > 5)
	{
		
	}
	else
	{
		Level = FMath::FloorToInt(Minute);
	}

	return Level;
}

