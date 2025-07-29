// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSpawner.h"
#include "HJungleSpawner.generated.h"

class AHAI_Character;

UCLASS()
class HGASTEMPLATE_API AHJungleSpawner : public AHSpawner
{
	GENERATED_BODY()

public:
	
	AHJungleSpawner();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="AProps")
	TSubclassOf<AHAI_Character> AICharacterClass;

	void SpawnAICharacter();
	
	void SpawnedCharacterDead();


	
};
