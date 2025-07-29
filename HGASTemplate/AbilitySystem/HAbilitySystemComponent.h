// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HGASTemplate/Data/HDataStructTypes.h"
#include "HGASTemplate/Interfaces/HAbilitySystemInterface.h"
#include "HAbilitySystemComponent.generated.h"

class UDynamicSkillInfoObject;
struct FHGameplayEffectContext;
class UHGameplayAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, FGameplayTagContainer& /*AssetTags*/)

/** Bir Abilityi ASC ye verdiğimiz zaman bu delegate bu ability bilgisini Broadcast edecek
 * ÖNEMLİ
 */
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);

DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);

DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);

USTRUCT(BlueprintType)
struct FSkillStatus
{
	GENERATED_BODY()
	
	FGameplayTag SkillTag;
	
	bool Status = false; // true = etkin, false = etkin değil

	FSkillStatus() {}

	FSkillStatus(FGameplayTag InTag, bool InStatus) : SkillTag(InTag), Status(InStatus) {}
};

USTRUCT(BlueprintType)
struct FUpgradeSkillInfoForOverlay
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag SkillTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentSkillLevel = 0;

	UPROPERTY(BlueprintReadOnly)
	bool UnlockedFirstTime = false; 

	FUpgradeSkillInfoForOverlay() {}

	FUpgradeSkillInfoForOverlay(FGameplayTag InTag, int32 InLevel, bool InUnlockedFirstTime) : SkillTag(InTag), CurrentSkillLevel(InLevel), UnlockedFirstTime(InUnlockedFirstTime) {}
	
};


/**
 * MarkAbilitySpecDirty() fonksiyonunu GA ye tag ekleyip çıkardığın zaman yada üstünde başka değişiklikler
 * yaptığında kullan
 */
UCLASS()
class HGASTEMPLATE_API UHAbilitySystemComponent : public UAbilitySystemComponent, public IHAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UHAbilitySystemComponent();

	/** HAbilitSystemInterface */
	virtual void I_CancelAllActionAbilities_Implementation() override;
	virtual float I_GetAttributeValueByTag_Implementation(const FGameplayTag& AttributeTag) override;
	/** HAbilitSystemInterface */

	void CancelAllActionAbilities();

	void AbilityActorInfoSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** pot laınca kristal alınca alınanın bilgileri yollayan delegate*/
	FEffectAssetTags EffectAssetTagsDelegate;

	FAbilitiesGiven AbilitiesGivenDelegate;
	bool bStartupAbilitiesGiven = false;

	FAbilityStatusChanged AbilityStatusChanged;

	FAbilityEquipped AbilityEquipped;

	FDeactivatePassiveAbility DeactivatePassiveAbility;
	
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	FGameplayAbilitySpec* GetAbilitySpecFromTag(const FGameplayTag& AbilityTag);
	
	UHGameplayAbility* GetHAbilityFromTag(const FGameplayTag& AbilityTag);
	UHGameplayAbility* GetSkillAbilityFromTag(const FGameplayTag& SkillTag);
	
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	float GetAttributeValueByTag(const FGameplayTag AttributeTag) const;
	
	/** Attributewidgetcontroller içindeki bir BPCallable fonk bunu çağırıyor*/
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

protected:
	/**
	 *Giveability() fonku ile abilityi serverde ASC ye verince aşağıdaki rep notify clientlerde firelanıyor
	 * Fired activatable abilities replicating.
	 */
	virtual void OnRep_ActivateAbilities() override;

	/** Serverden çalışan bir delegateye eklenmiştir. */
	UFUNCTION(Client,Reliable)
	void Client_EffectApplied(UAbilitySystemComponent*AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);

public:
	
	void ActivateNormalAttackAbility();
	void EndNormalAttackAbility();
	UGameplayAbility* GetNormalAttackAbility();

	/**
	 * Skill Works
	 */
	
	/** Checks if the given ability tag corresponds to a skill.
	 *
	 * @param AbilityTag The gameplay tag of the ability being checked.
	 * @return True if the ability tag matches one of the predefined skill tags, otherwise false.
	 */
	bool IsAbilitySkill(const FGameplayTag& AbilityTag); 

	void UpgradeSkillLevel(const FGameplayTag& SkillTag);
	void UpgradeCharacterPassiveSkillLevel();

	bool IsSkillUnlocked(const FGameplayTag& SkillTag);

	UFUNCTION(Client,Reliable)
	void Client_SendUpgradeSkillInfo(const FUpgradeSkillInfoForOverlay& Info);
	
	FGameplayEffectSpec* CreateTimedTagEffectSpec(const FGameplayTag& Tag, float EffectDuration);
	void CreateTimedTagEffectSpecAndApply(const FGameplayTag& Tag, float EffectDuration);

	
	void UpgradeSkillLevelByTag(const FGameplayTag SkillTag);
	
	/*UPROPERTY()
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilitiesClassArray;*/ // ?

	/**
	 * DynamicSkillInfo For Tooltips
	 */

	UPROPERTY()
	TArray<UDynamicSkillInfoObject*> DynamicSkillInfoObjectsArray;

	UFUNCTION(Client,Reliable)
	void Client_InitializeClientDynamicInfoObject(TSubclassOf<UDynamicSkillInfoObject> DynamicSkillInfoObjectClass);

	UDynamicSkillInfoObject* GetDynamicSkillInfoObjectByTag(const FGameplayTag& SkillTag);

	FString GetSkillDescriptionByTag(const FGameplayTag& SkillTag);
	FDynamicSkillInfo GetDynamicSkillInfoByTag(const FGameplayTag& SkillTag);

	UFUNCTION(Client,Reliable)
	void Client_UpdateDynamicSkillInfoObjectLevel(const FGameplayTag& SkillTag, int32 NewLevel);

	


	
	
};
