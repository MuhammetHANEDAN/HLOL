// Fill out your copyright notice in the Description page of Project Settings.


#include "HJungleSpawner.h"

#include "HGASTemplate/Characters/HAI_Character.h"


// Sets default values
AHJungleSpawner::AHJungleSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHJungleSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() && bCanSpawn)
	{
		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH,this,&AHJungleSpawner::SpawnAICharacter,FirstDelay);
	}
	
}

// Called every frame
void AHJungleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHJungleSpawner::SpawnAICharacter()
{
	if (HasAuthority() == false) return;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	
	AHAI_Character* SpawnedCharacter = GetWorld()->SpawnActorDeferred<AHAI_Character>(AICharacterClass,SpawnTransform,this,
		nullptr,ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	SpawnedCharacter->Level = CalculateLevelForTime(GetGameTimeSeconds());

	SpawnedCharacter->Spawner = this;
	
	SpawnedCharacter->FinishSpawning(SpawnTransform);
	
}

void AHJungleSpawner::SpawnedCharacterDead()
{
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&AHJungleSpawner::SpawnAICharacter,SpawnFrequency);
}

