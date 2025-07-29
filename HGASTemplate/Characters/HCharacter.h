// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "HGASTemplate/Characters/HBaseCharacter.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "HCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTargetActorLost);

class AHPlayerController;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API AHCharacter : public AHBaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	AHCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	/** Players Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToGold_Implementation(int32 InGold) override;
	/** end Player Interface */

	/** Combat Interface */
	virtual AActor* GetTargetCharacter_Implementation() override;
	virtual float GetXPReward_Implementation() override;
	virtual UHAttributeSet* I_GetHAS_Implementation() override;
	virtual int32 GetGoldReward_Implementation() override;
	virtual void Die(const FVector& DeathImpulse, AActor* KillerActor) override;
	virtual void ReSpawn(const FVector& Location, const FRotator& Rotation) override;

	
	virtual int32 GetPlayerLevel_Implementation() override;
	/** Combat Interface */

	/**
	 * Ability System Works
	 */
	
	void Delay_SetAbilitySystemComponent();

	virtual void AbilitySystemInitialized(const UAbilitySystemComponent* ASC) override;

	virtual void InitializeControllerAS() override;
	bool bHplayerControllerASInitialized = false;

private:
	
	virtual void InitAbilityActorInfo() override;

public:

	UPROPERTY()
	TObjectPtr<AHPlayerController> HPlayerController;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_TargetActor)
	TObjectPtr<AActor> TargetActor;
	void SetTargetActor(AActor* TActor);
	UFUNCTION()
	void OnRep_TargetActor();
	FOnTargetActorLost TargetActorLostDelegate;

	

	/**
	 * Reset Works
	 */

	/** Karakter yeniden doğunca yapılması gereken işlemler*/
	void ResetCharacterForRespawn();

	/**
	 * Bar Component
	 */

	UPROPERTY(VisibleAnywhere,Category="AProps")
	TObjectPtr<UBarComponent> BarComponent;

	void Delayed_HideLocalBar();

	/**
	 * ReSpawn
	 */

	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_HandleReSpawn();
	UPROPERTY(EditDefaultsOnly,Category="AProps")
	float DeathToRespawnZoneDelay = 4.f;

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TObjectPtr<UAnimMontage> ReSpawnMontage;

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TObjectPtr<UAnimMontage> RecallMontage;
	
	void DeathToWaitRespawnZone();

	virtual void Multicast_HandleDeath(const FVector& DeathImpulse) override;

	/**
	 * Camera
	 */

	void SetCameraLocationToCharacter();

	/**
	 * Debug ------------ TODO Silinecekler -----------------------------------------------------------------------
	 */

	UFUNCTION(BlueprintCallable,Category="Debug")
	void Debug_SetTeam(bool bRadiant);

	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Debug")
	void Server_Debug_AddXp(float Value);
	
	UFUNCTION(Server,Reliable,BlueprintCallable,Category="Debug")
	void Server_Debug_AddGold();
	
	
};
