// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSpawner.h"
#include "HMininonSpawner.generated.h"

class AHAI_Character;

UCLASS()
class HGASTEMPLATE_API AHMininonSpawner : public AHSpawner
{
	GENERATED_BODY()

public:
	
	AHMininonSpawner();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="AProps")
	float FrequencyBeetwenWaves = 10.f;
	FTimerHandle WaveTH;

	UPROPERTY(EditAnywhere,Category="AProps")
	FVector EndTargetLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere,Category="AProps")
	TSubclassOf<AHAI_Character> MeleeMinionClass;

	UPROPERTY(EditAnywhere,Category="AProps")
	TSubclassOf<AHAI_Character> RangedMinionClass;

	int32 SpawnedMeleeMinionsOnWave = 0;
	int32 SpawnedRangedMinionsOnWave = 0;

	UPROPERTY(EditAnywhere,Category="AProps")
	int32 MaxSpawnMeleeMinionsCountPerWave = 3;
	UPROPERTY(EditAnywhere,Category="AProps")
	int32 MaxSpawnRangedMinionsCountPerWave = 3;

	FTimerHandle MeleeTH;
	FTimerHandle RangedTH;


	void StartProcess();
	void WaveSpawnProcessOver();

	void SpawnMinion(TSubclassOf<AHAI_Character> MinionClass);

	void StartSpawnMeleeMinion();
	void SpawnMeleeMinion();
	
	void StartSpawnRangedMinion();
	void SpawnRangedMinion();

	
	









	
};
