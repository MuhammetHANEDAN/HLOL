
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

struct FUpgradeSkillInfoForOverlay;
// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HGASTEMPLATE_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AnimEvent_HandleNormalAttackAbility();

	UFUNCTION(BlueprintNativeEvent)
	void UpgradeSkillInfoSend(const FUpgradeSkillInfoForOverlay& Info);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ContinueNormalAttack();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AddToGold(int32 InGold);

	UFUNCTION(BlueprintNativeEvent)
	void I_AddToKillCount();
	UFUNCTION(BlueprintNativeEvent)
	void I_AddToDeathCount();
	UFUNCTION(BlueprintNativeEvent)
	void I_AddToLastHitCount();

	UFUNCTION(BlueprintNativeEvent)
	void GameStarted();

	UFUNCTION(BlueprintNativeEvent)
	void InitializeGameTime(float Time);

	UFUNCTION(BlueprintNativeEvent)
	AActor* GetTargetActor();

	UFUNCTION(BlueprintNativeEvent)
	void I_ShowKillWidget(const FGameplayTag& KillerTag, const FGameplayTag& VictimTag);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void I_ResetHPlayerControllerMovementAndTarget();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void I_SetCameraLocationToPawn();


	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckpointTag);
};

