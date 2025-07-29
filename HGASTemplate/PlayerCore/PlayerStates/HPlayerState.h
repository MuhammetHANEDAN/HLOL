// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerState.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "HPlayerState.generated.h"

struct FUpgradeSkillInfoForOverlay;
class UHAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FSkillUpgradeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag SkillTag;

	UPROPERTY(BlueprintReadWrite)
	bool bActivate = false;

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentSkillLevel = 0;
};

USTRUCT(BlueprintType)
struct FInventoryItemStruct
{
	GENERATED_BODY()
	
	FInventoryItemStruct() : SlotNumber(0) {} 
	FInventoryItemStruct(int32 Number) : SlotNumber(Number) {}

	UPROPERTY(BlueprintReadOnly)
	bool bSlotIsEmpty = true;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	int32 SlotNumber = 1;
	
};

class UData_AbilityInfo;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpgradeButton, TArray<FSkillUpgradeInfo> )

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpgradeSkillInfoSignature2, const FUpgradeSkillInfoForOverlay&)

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemStructSignature, const FInventoryItemStruct& )

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameTimeChanged, float /*StatValue*/)

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API AHPlayerState : public APlayerState, public IAbilitySystemInterface, public IPlayerInterface
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	AHPlayerState();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeSet> AttributeSet;


	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	UHAbilitySystemComponent* GetHASC() const;

	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void UpgradeSkillInfoSend_Implementation(const FUpgradeSkillInfoForOverlay& Info) override;
	virtual void AddToGold_Implementation(int32 InGold) override;
	virtual void I_AddToKillCount_Implementation() override;
	virtual void I_AddToDeathCount_Implementation() override;
	virtual void I_AddToLastHitCount_Implementation() override;
	virtual void InitializeGameTime_Implementation(float Time) override;
	/** Player Interface */

	/** Xp percent bilgisi gidiyor */
	FOnPlayerStatChanged OnXPChangedDelegate;
	
	FOnLevelChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	FOnUpgradeButton UpgradeButtonDelegate;

	FOnUpgradeSkillInfoSignature2 UpgradeSkillInfoDelegate;

	FOnPlayerStatChanged GoldChangedDelegate;
	
	void AddToXP(int32 InXP);
	
	void AddToLevel(int32 InLevel);
	void UpgradeLevel(); // Levele 1 ekler
	UFUNCTION(Server,Reliable)
	void Server_UpgradeLevel();
	
	void AddToAttributePoints(int32 InPoints);
	void UpgradeAttributePoints(); // Attribute pointse 1 ekler
	
	void AddToSpellPoints(int32 InPoints);
	void UpgradeSpellPoints(); // Spell Pointse 1 ekler

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);

	UPROPERTY(EditDefaultsOnly,Category="AProperty")
	TObjectPtr<UData_AbilityInfo> AbilityInfoDataAsset;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 1;
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing=OnRep_Gold)
	int32 Gold = 5000;
	UFUNCTION()
	void OnRep_Gold(int32 OldGold);
	int32 GetGold() {return Gold;}

	// TODO geliştrilecek
	void AddGold();
	

	int32 FindLevelForExp(float InXP);
	float GetCurrentXPPercent();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AProps")
	TArray<float> TotalLevelExpArray;

	/**
	 * SKILL POINTS WORKS
	 */

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	TMap<FGameplayTag,int32> SkillLevelMap;

	uint8 NormalSkillMaxLevel = 5;
	uint8 UltiSkillMaxLevel = 3;

	/** Skill Tag = Q = InputTag_Q */
	UFUNCTION(Server,Reliable)
	void Server_UpgradeSkill(const FGameplayTag& SkillTag);

	void HandleSkillPointsForUpgradeButtons();

	/** Level atlama sonrası veya upgradeskill çalıştırıldığında bu client rpc çalıştırılacak ve overlaywidgetcontrollera
	 * hangi upgrade tuşlarının aktif veya deaktif edileceği bilgisi gönderilecektir
	 */
	UFUNCTION(Client,Reliable)
	void Client_SendUpgradeButtonsWorks(const TArray<FSkillUpgradeInfo>& UpgradeInfos);

	/**
	 * INVENTORY WORKS
	 */

	// Sadece Serverde Ekleme ve Çıkarma Yapılmalı

	FOnInventoryItemStructSignature OnInventoryItemStructDelegate;

	UPROPERTY()
	TArray<FInventoryItemStruct> InventorySlotArray;

	bool InventoryHaveEmptySlot();
	bool HaveEnoughGoldForBuyItem(const FGameplayTag& ItemTag);
	
	void AddItem(const FGameplayTag& ItemTag);
	bool bBuyingItem = false;
	void BuyItem(const FGameplayTag& ItemTag);

	UFUNCTION(Server,Reliable)
	void Server_AddItem(const FGameplayTag& ItemTag);

	UFUNCTION(Client,Reliable)
	void Client_SendItemInfo(const FInventoryItemStruct& ItemStruct);

	void InitializeItemAbility(const FGameplayTag& ItemTag);

	//------------ Free Space

	UPROPERTY(ReplicatedUsing=OnRep_KillCount,VisibleDefaultsOnly,BlueprintReadOnly,Category="AProps")
	int32 KillCount = 0;
	void AddKillCount();
	UFUNCTION()
	void OnRep_KillCount();

	FOnPlayerStatChanged OnKillCountChangedDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_DeathCount,VisibleDefaultsOnly,BlueprintReadOnly,Category="AProps")
	int32 DeathCount = 0;
	void AddDeathCount();
	UFUNCTION()
	void OnRep_DeathCount();

	FOnPlayerStatChanged OnDeathCountChangedDelegate;

	UPROPERTY(ReplicatedUsing=OnRep_LastHitCount,VisibleDefaultsOnly,BlueprintReadOnly,Category="AProps")
	int32 LastHitCount = 0;
	void AddLastHitCount();
	UFUNCTION()
	void OnRep_LastHitCount();

	FOnPlayerStatChanged OnLastHitCountChangedDelegate;

	/**
	 * TIME SYNC
	 */
	

	/** Auth Player oyunun başında gidip servertime ı alır ve Cliente çoğaltır */
	float GameTime = 0.f;

	UFUNCTION(Client,Reliable)
	void Client_ReceivedTime(float L_GameTime);

	UFUNCTION(Server,Reliable)
	void Server_ReportTime();
	
	/** Servere yollanan rpc den sonra serverden cliente dönen rpc ye kadar geçen zamanın bulunması için
	 *  Servere yollanınca true serverden rpc alınınca false olacak.
	 */
	bool bServerRpcSendedTrack = false;
	float LastRoundTripTime = 0.f;
	float RoundTripTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Properties")
	float CheckTimeSyncFrequency = 0.5f;
	float CheckTimeSyncFrequencyTrack = 0.f;
	bool bTimeSycned = false;

	FOnGameTimeChanged OnGameTimeChangedDelegate;

	UFUNCTION(Client,Reliable)
	void Client_SetTimeFromServer(float Time);

	//-------------------

	UPROPERTY()
	bool bIsRadiant = true;

	UPROPERTY()
	FGameplayTag HeroTag = FHGamePlayTags::Get().Heroes_Grux;

	UFUNCTION(Server,Reliable)
	void Server_SetIsRadiantPlayer(bool bRadiant);

	UFUNCTION(Server,Reliable)
	void Server_SetHeroTag(FGameplayTag tag);

public:
	
	FORCEINLINE int32 GetPlayerLevel() const {return Level;}
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
};
