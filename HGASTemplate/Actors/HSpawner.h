// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSpawner.generated.h"

class UArrowComponent;

UCLASS()
class HGASTEMPLATE_API AHSpawner : public AActor
{
	GENERATED_BODY()

public:
	
	AHSpawner();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent> ArrowComponent;

	UPROPERTY(EditAnywhere,Category="AProps")
	bool bCanSpawn = true;

	UPROPERTY(EditAnywhere,Category="AProps")
	float FirstDelay = 5;

	UPROPERTY(EditAnywhere,Category="AProps")
	float SpawnFrequency = 5.f;

	UFUNCTION(BlueprintPure)
	float GetGameTimeSeconds() const;

	int32 CalculateLevelForTime(float TimeSeconds);

	
};
