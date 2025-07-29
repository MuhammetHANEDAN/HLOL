
#include "HMininonSpawner.h"

#include "HGASTemplate/Characters/HAI_Character.h"
#include "Kismet/GameplayStatics.h"


AHMininonSpawner::AHMininonSpawner()
{
	
	PrimaryActorTick.bCanEverTick = true;
}

void AHMininonSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHMininonSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority() == false || bCanSpawn == false) return;

	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&AHMininonSpawner::StartProcess,FirstDelay);
	
}

void AHMininonSpawner::StartProcess()
{
	StartSpawnMeleeMinion();
}

void AHMininonSpawner::WaveSpawnProcessOver()
{
	GetWorldTimerManager().SetTimer(WaveTH,this,&ThisClass::StartProcess,FrequencyBeetwenWaves);
	SpawnedMeleeMinionsOnWave = 0;
	SpawnedRangedMinionsOnWave = 0;
}

void AHMininonSpawner::SpawnMinion(TSubclassOf<AHAI_Character> MinionClass)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GetActorLocation());
	SpawnTransform.SetRotation(GetActorRotation().Quaternion());
	
	AHAI_Character* SpawnedMinion = GetWorld()->SpawnActorDeferred<AHAI_Character>(MinionClass,SpawnTransform,this,
		nullptr,ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	
	SpawnedMinion->Level = CalculateLevelForTime(GetGameTimeSeconds());

	SpawnedMinion->EndTargetLocation = EndTargetLocation;
	SpawnedMinion->FinishSpawning(SpawnTransform);
	
}

void AHMininonSpawner::StartSpawnMeleeMinion()
{
	GetWorldTimerManager().SetTimer(MeleeTH,this,&ThisClass::SpawnMeleeMinion,SpawnFrequency,true);
}

void AHMininonSpawner::SpawnMeleeMinion()
{
	if (SpawnedMeleeMinionsOnWave == MaxSpawnMeleeMinionsCountPerWave)
	{
		GetWorldTimerManager().ClearTimer(MeleeTH);
		StartSpawnRangedMinion();
	}
	else
	{
		SpawnMinion(MeleeMinionClass);
		SpawnedMeleeMinionsOnWave++;
	}
}

void AHMininonSpawner::StartSpawnRangedMinion()
{
	GetWorldTimerManager().SetTimer(RangedTH,this,&ThisClass::SpawnRangedMinion,SpawnFrequency,true,0);
}

void AHMininonSpawner::SpawnRangedMinion()
{
	if (SpawnedRangedMinionsOnWave == MaxSpawnRangedMinionsCountPerWave)
	{
		GetWorldTimerManager().ClearTimer(RangedTH);
		WaveSpawnProcessOver();
	}
	else
	{
		SpawnMinion(RangedMinionClass);
		SpawnedRangedMinionsOnWave++;
	}
}




