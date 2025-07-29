// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "HGASTemplate/Characters/HCharacter.h"
#include "HGASTemplate/Interfaces/HGameModeInterface.h"
#include "HGameMode.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API AHGameModeBase : public AGameModeBase, public IHGameModeInterface
{
	GENERATED_BODY()

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaSeconds) override;

	/** IHGameModeInterface*/
	virtual void I_KillEvent(AActor* KillerActor, AActor* KilledActor) override;
	virtual float I_GetGameTime() override;
	/** IHGameModeInterface*/

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TMap<FGameplayTag,TSubclassOf<AHCharacter>> TagToCharacterClassMap;

	void KillEvent(AActor* KillerActor, AActor* KilledActor);

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	float DelayToStart = 5.f;

	/**
	 * Respawn Players ----------- Aslında Bir spawn işlemi yoktur. Player karakterler yok edilmiyor. 
	 */
	
	float GetPlayerReSpawnCooldown() const;
	
	void StartReSpawnPlayerCharacter(AActor* KilledActor);
	UFUNCTION()
	void ReSpawnPlayerCharacter(AActor* KilledActor);


	/**
	 * TIME SYNC
	 */

	bool GameStarted = false;
	float GameTime = 0.f;

	/** PC lere Delay Widgetinin kaldırmaları emri gider.
	 * PS lere Game Time ı iletir
	 */
	void StartGame();

	TArray<APlayerController*> GetActivePlayerControllers();
	TArray<APlayerState*> GetActivePlayerStates();

	/**
	 * -------------------------------
	 */

	virtual void RestartPlayer(AController* NewPlayer) override;
	UFUNCTION()
	void RestartPlayerDelayed(AController* NewPlayer);
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	AActor* FindHPlayerStartForTeam(bool bRadiant);
	AActor* FindHPlayerRespawnLocationForTeam(bool bRadiant);
	AHCharacter* SpawnHCharacter(AController* NewPlayer, AActor* StartSpot, TSubclassOf<AHCharacter> CharacterClass);
	









	
	
};
