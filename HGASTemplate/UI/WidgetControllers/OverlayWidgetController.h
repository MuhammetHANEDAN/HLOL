// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HWidgetController.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/Data/Data_Hero.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"
#include "OverlayWidgetController.generated.h"


class UMarketWidgetController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeButtonsSignature, bool, bUnlocked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeSkillInfoSignature, const FUpgradeSkillInfoForOverlay&, UpgradeSkillInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryItemStructSignature2, const FInventoryItemStruct&, ItemArrayInfo);

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UOverlayWidgetController : public UHWidgetController
{
	GENERATED_BODY()

public:

	UOverlayWidgetController();

	/** BP Callable*/
	virtual void BroadcastInitialValues() override;
	
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerAttributeChangedSignature AttributeChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature LevelChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature SpellPointDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature GoldChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemStructSignature2 OnInventoryItemStructDelegate;

	UFUNCTION(BlueprintCallable)
	FHeroDataStruct& GetHeroData() const;

	

	/**
	 * Upgrade Buttons Works
	 */

	UPROPERTY()
	TMap<FGameplayTag,FOnUpgradeButtonsSignature> UpgradeButtonDelegateMap;

	void InitializeUpgradeButtonDelegateMap();
	
	UPROPERTY(BlueprintAssignable)
	FOnUpgradeButtonsSignature UpgradeButtonQDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUpgradeButtonsSignature UpgradeButtonWDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnUpgradeButtonsSignature UpgradeButtonEDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnUpgradeButtonsSignature UpgradeButtonRDelegate;

	/** Skill Upgrade Level*/

	UPROPERTY(BlueprintAssignable)
	FOnUpgradeSkillInfoSignature SkillUnlockedDelegate;

	

	/** Skill Tag = Q = InputTag_Q */
	UFUNCTION(BlueprintCallable)
	void UpgradeButtonClicked(const FGameplayTag& SkillTag);
	
	void UpgradeButtonsWorks(const TArray<FSkillUpgradeInfo>& UpgradeInfos);
	

	UFUNCTION(BlueprintCallable)
	UMarketWidgetController* MakeMarketWidgetControllerWithPSPC();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMarketWidgetController> MarketWidgetControllerClass;

	// -------------------------

	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature OnKillCountChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature OnDeathCountChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature OnLastHitCountChangedDelegate;


	UPROPERTY(BlueprintAssignable)
	FOnFloatStatChangedSignature OnGameTimeChangedDelegate;

	
};
