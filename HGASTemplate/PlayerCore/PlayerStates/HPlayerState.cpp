
#include "HPlayerState.h"

#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Data/Data_Item.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "HGASTemplate/Interfaces/HGameModeInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameModeBase.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"

void AHPlayerState::BeginPlay()

{
	if(HasAuthority())
	{
		FHGamePlayTags HTags = FHGamePlayTags::Get();
		SkillLevelMap.Add(HTags.InputTag_Q,0);
		SkillLevelMap.Add(HTags.InputTag_W,0);
		SkillLevelMap.Add(HTags.InputTag_E,0);
		SkillLevelMap.Add(HTags.InputTag_R,0);

		//TODO Geliştirliecek
		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH,this, &AHPlayerState::AddGold,3,true); //Sıkıntı nerdeeeeeee nerdeeee aptalca bi bug var clientte

		bTimeSycned = true;
		
	}
	
	else
	{
		bServerRpcSendedTrack = true;
		Server_ReportTime();
	}


	/*if (GetPlayerController()->IsLocalController())
	{
		bServerRpcSendedTrack = true;
		Server_ReportTime();
	}*/

	
	
	
}

AHPlayerState::AHPlayerState()
{
	AbilitySystemComponent =CreateDefaultSubobject<UHAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UHAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;

	InventorySlotArray.Add(FInventoryItemStruct(1));
	InventorySlotArray.Add(FInventoryItemStruct(2));
	InventorySlotArray.Add(FInventoryItemStruct(3));
	InventorySlotArray.Add(FInventoryItemStruct(4));
	InventorySlotArray.Add(FInventoryItemStruct(5));
	InventorySlotArray.Add(FInventoryItemStruct(6));

	
	
}

void AHPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		CheckTimeSyncFrequencyTrack += DeltaSeconds;
		if(CheckTimeSyncFrequencyTrack >= CheckTimeSyncFrequency)
		{
			
			CheckTimeSyncFrequencyTrack = 0;
			
			AGameModeBase* BaseM = UGameplayStatics::GetGameMode(this);
			check(BaseM);
	
			if (IHGameModeInterface* IC = Cast<IHGameModeInterface>(BaseM))
			{
				GameTime = IC->I_GetGameTime();
				OnGameTimeChangedDelegate.Broadcast(GameTime);
			}
				
		}
	}

	else
	{
		if(bServerRpcSendedTrack) 
		{
			RoundTripTime += DeltaSeconds;
		}

		if(bTimeSycned) 
		{
			CheckTimeSyncFrequencyTrack += DeltaSeconds;
			if(CheckTimeSyncFrequencyTrack >= CheckTimeSyncFrequency)
			{
				Server_ReportTime();
				bTimeSycned = false;
			}
		}
	}
	
	
	
}

void AHPlayerState::Server_ReportTime_Implementation()
{
	AGameModeBase* BaseM = UGameplayStatics::GetGameMode(this);
	check(BaseM);
	
	if (IHGameModeInterface* IC = Cast<IHGameModeInterface>(BaseM))
	{
		return Client_ReceivedTime(IC->I_GetGameTime());
	}
	
	return Client_ReceivedTime(0);
}

void AHPlayerState::Client_ReceivedTime_Implementation(float L_GameTime)
{
	
	/*bServerRpcSendedTrack = false;

	GameTime = L_GameTime + (RoundTripTime/2);
	OnGameTimeChangedDelegate.Broadcast(GameTime);

	bTimeSycned = true;

	HLOG("Client Rpc Geldi")
	HLOGF(GameTime);*/

	bServerRpcSendedTrack = false; 
	GameTime = L_GameTime +(RoundTripTime/2);
	OnGameTimeChangedDelegate.Broadcast(GameTime);

	LastRoundTripTime = RoundTripTime;
	
	RoundTripTime = 0;
	CheckTimeSyncFrequencyTrack = 0;
	bTimeSycned = true;
	
}

void AHPlayerState::InitializeGameTime_Implementation(float Time)
{
	Client_SetTimeFromServer(Time);
}

void AHPlayerState::Client_SetTimeFromServer_Implementation(float Time)
{
	GameTime = Time + LastRoundTripTime;
	OnGameTimeChangedDelegate.Broadcast(GameTime);
}

void AHPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHPlayerState,Level);
	DOREPLIFETIME(AHPlayerState, XP);
	DOREPLIFETIME(AHPlayerState, AttributePoints);
	DOREPLIFETIME(AHPlayerState, SpellPoints);
	DOREPLIFETIME(AHPlayerState, Gold);
	DOREPLIFETIME_CONDITION(AHPlayerState,KillCount,COND_OwnerOnly); 
	DOREPLIFETIME_CONDITION(AHPlayerState,DeathCount,COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AHPlayerState,LastHitCount,COND_OwnerOnly);
}

UAbilitySystemComponent* AHPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UHAbilitySystemComponent* AHPlayerState::GetHASC() const
{
	if (AbilitySystemComponent)
	{
		return Cast<UHAbilitySystemComponent>(AbilitySystemComponent);
	}
	return nullptr;
}

void AHPlayerState::AddToXP_Implementation(int32 InXP) // Sadece serverda geldiğinden eminiz
{
	const int32 CurrentLevel = Level;
	const int32 CurrentXP = XP + InXP;
	XP = CurrentXP;
	OnXPChangedDelegate.Broadcast(GetCurrentXPPercent());
		
	const int32 NewLevel = FindLevelForExp(XP);
	const int32 NumLevelUps = NewLevel - CurrentLevel;
	
	if (NumLevelUps > 0) // Level alınmış ise
	{
		for (int32 i = 0; i < NumLevelUps; i++)
		{
			//UpgradeLevel();
			Server_UpgradeLevel();
			UpgradeSpellPoints();
		}
		
		OnXPChangedDelegate.Broadcast(GetCurrentXPPercent());

		HandleSkillPointsForUpgradeButtons();

		/* Bunlar düzgün çalışmıyodu çünkü Level atlandığında MMC_HealthRegen buradan PostExecuteden sonra çalışıyor
			 * ve Maxhelathı ve Maxmanayı arttırıyor burda setlersek max değerler olması gerekenden daha düşük kalıyor
			 * yani bir önceki levelin değerinde kalıyor o yüzden PostAttributeChange kullanıldı
			SetHealthRegen(GetMaxHealthRegen());
			SetMana(GetMaxMana());*/ 
			
		/*bTopOffHealth= true;
		bTopOffMana = true;*/
			
		//IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
	}
}

void AHPlayerState::UpgradeSkillInfoSend_Implementation(const FUpgradeSkillInfoForOverlay& Info)
{
	UpgradeSkillInfoDelegate.Broadcast(Info);
}

void AHPlayerState::AddToGold_Implementation(int32 InGold)
{
	Gold += InGold;
	GoldChangedDelegate.Broadcast(Gold);
}

void AHPlayerState::I_AddToKillCount_Implementation()
{
	AddKillCount();
}

void AHPlayerState::I_AddToDeathCount_Implementation()
{
	AddDeathCount();
}

void AHPlayerState::I_AddToLastHitCount_Implementation()
{
	AddLastHitCount();
}

int32 AHPlayerState::FindLevelForExp(float InXP)
{
	if(InXP < TotalLevelExpArray[1]) return 1; // 2. levele geçmek için gereken exp miktarından az ise level atlamadı demektir

	// LevelEXP Map ---- level için gerekli total exp mantığına dayanıyoruz
	// Max level 18 dir yani 0 dan 17 ye git
	
	// 0 1 0
	// 1 2 280
	// 2 3 660
	// 3 4 1140

	// diyelimki 700 exp var
	
	// XP search edeceğiz.
	// İlk olarak Xp hangi levellerden büyük onu bul
	// Sonra ilerle xp hangi levelin XP sinden küçük onu bul o istenilen leveldir

	// 1 - 3 atladı diyelim

	bool bSearching = true;
	int32 LocalLevel = 0;
	while (bSearching)
	{
		if(LocalLevel >= 17) return 18; // LocalLevel >= 17 sonradan görünce yanlış anlama, index olarak 18 e gidiyor, 0 dan başladığı için
		if(InXP >= TotalLevelExpArray[LocalLevel] ) LocalLevel ++;
		else
		{
			return LocalLevel;
		}
	}
	return 1;
}

float AHPlayerState::GetCurrentXPPercent()
{
	if(Level == 1) return XP / TotalLevelExpArray[1];
	if(Level == 18) return XP / TotalLevelExpArray[17];
	float CurrentMaxXP = TotalLevelExpArray[Level];
	return XP / CurrentMaxXP;
}

void AHPlayerState::Server_UpgradeSkill_Implementation(const FGameplayTag& SkillTag)
{
	if(SpellPoints == 0) return;
	SpellPoints = FMath::Clamp(SpellPoints-1,0,18);
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
	
	int32* SkillLevel = SkillLevelMap.Find(SkillTag);
	(*SkillLevel)++;
	
	HandleSkillPointsForUpgradeButtons(); // İçinde client rpc var
	
	check(GetHASC());
	GetHASC()->UpgradeSkillLevel(SkillTag); // ASC deki ability level arttır
	
}

void AHPlayerState::HandleSkillPointsForUpgradeButtons()
{
	TArray<FSkillUpgradeInfo> UpgradeInfos;
	const FHGamePlayTags& HTags = FHGamePlayTags::Get();

	constexpr int32 RUnlockLevel1 = 6;
	constexpr int32 RUnlockLevel2 = 11;
	constexpr int32 RUnlockLevel3 = 16;

	const FGameplayTag TagsToCheck[] = { HTags.InputTag_Q, HTags.InputTag_W, HTags.InputTag_E };

	// Normal skiller
	for (const FGameplayTag& SkillTag : TagsToCheck)
	{
		const int32* CurrentLevelPtr = SkillLevelMap.Find(SkillTag);
		const bool bCanUpgrade = (SpellPoints > 0 && CurrentLevelPtr && *CurrentLevelPtr < NormalSkillMaxLevel);

		FSkillUpgradeInfo Info;
		Info.SkillTag = SkillTag;
		Info.bActivate = bCanUpgrade;
		UpgradeInfos.Add(Info);
	}

	// Ulti kontrolü
	{
		const int32* RLevelPtr = SkillLevelMap.Find(HTags.InputTag_R);
		bool bCanUpgradeR = false;

		if (RLevelPtr)
		{
			const int32 RLevel = *RLevelPtr;

			if ((Level >= RUnlockLevel1 && RLevel < 1) ||
				(Level >= RUnlockLevel2 && RLevel < 2) ||
				(Level >= RUnlockLevel3 && RLevel < 3))
			{
				bCanUpgradeR = SpellPoints > 0;
			}
		}

		FSkillUpgradeInfo Info;
		Info.SkillTag = HTags.InputTag_R;
		Info.bActivate = bCanUpgradeR;
		UpgradeInfos.Add(Info);
	}
	
	Client_SendUpgradeButtonsWorks(UpgradeInfos);
}

void AHPlayerState::Client_SendUpgradeButtonsWorks_Implementation(const TArray<FSkillUpgradeInfo>& UpgradeInfos)
{
	UE_LOG(LogTemp,Error,TEXT("hp client_Sendupgradebuttonsworks çalıştı"))
	UpgradeButtonDelegate.Broadcast(UpgradeInfos);
}

void AHPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AHPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AHPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AHPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void AHPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AHPlayerState::UpgradeLevel()
{
	if (Level >= 18) return;
	Level ++;
	OnLevelChangedDelegate.Broadcast(Level, true);
	
	GetHASC()->UpgradeCharacterPassiveSkillLevel();
}

void AHPlayerState::Server_UpgradeLevel_Implementation()
{
	if (Level >= 18) return;
	Level ++;
	OnLevelChangedDelegate.Broadcast(Level, true);
	
	GetHASC()->UpgradeCharacterPassiveSkillLevel();
}

void AHPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AHPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AHPlayerState::UpgradeAttributePoints()
{
	AttributePoints ++;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AHPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AHPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AHPlayerState::UpgradeSpellPoints()
{
	SpellPoints ++;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AHPlayerState::OnRep_Gold(int32 OldGold)
{
	GoldChangedDelegate.Broadcast(Gold);
}

void AHPlayerState::AddGold()
{
	if (bBuyingItem) return;
	Gold = GetGold() + 1;
	GoldChangedDelegate.Broadcast(Gold);
}

void AHPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

bool AHPlayerState::InventoryHaveEmptySlot()
{
	for (auto& Element : InventorySlotArray)
	{
		if (Element.bSlotIsEmpty) return true;
	}
	return false;
}

bool AHPlayerState::HaveEnoughGoldForBuyItem(const FGameplayTag& ItemTag)
{
	UData_Item* Data = UHBlueprintFunctionLibrary::GetItemDataAsset(this);
	check(Data);

	float Price = Data->GetItemDataFromTag(ItemTag).BuyPrice;

	return Gold >= Price;
}

void AHPlayerState::BuyItem(const FGameplayTag& ItemTag)
{
	bBuyingItem = true;
	
	UData_Item* Data = UHBlueprintFunctionLibrary::GetItemDataAsset(this);
	check(Data);

	float Price = Data->GetItemDataFromTag(ItemTag).BuyPrice;

	Gold = Gold -  Price;
	GoldChangedDelegate.Broadcast(Gold);

	bBuyingItem = false;
}

void AHPlayerState::AddItem(const FGameplayTag& ItemTag)
{
	/*if (InventoryHaveEmptySlot() && HaveEnoughGoldForBuyItem(ItemTag))
	{
		for (auto& Element : InventorySlotArray)
		{
			if (Element.bSlotIsEmpty)
			{
				Element.ItemTag = ItemTag;
				Element.bSlotIsEmpty = false;
				Server_AddItem(ItemTag);
				return;
			}
		}
	}*/

	if (HaveEnoughGoldForBuyItem(ItemTag))
	{
		Server_AddItem(ItemTag);
	}
	
}

void AHPlayerState::Server_AddItem_Implementation(const FGameplayTag& ItemTag)
{
	if (InventoryHaveEmptySlot() && HaveEnoughGoldForBuyItem(ItemTag))
	{
		for (auto& Element : InventorySlotArray)
		{
			if (Element.bSlotIsEmpty)
			{
				Element.ItemTag = ItemTag;
				Element.bSlotIsEmpty = false;

				Client_SendItemInfo(Element);

				BuyItem(ItemTag);
				InitializeItemAbility(ItemTag);
				return;
			}
		}
	}
}

void AHPlayerState::Client_SendItemInfo_Implementation(const FInventoryItemStruct& ItemStruct)
{
	OnInventoryItemStructDelegate.Broadcast(ItemStruct);
}

void AHPlayerState::InitializeItemAbility(const FGameplayTag& ItemTag)
{
	UData_Item* Data = UHBlueprintFunctionLibrary::GetItemDataAsset(this);
	check(Data);

	if (Data->GetItemDataFromTag(ItemTag).ItemAbilityClass == nullptr) return;

	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Data->GetItemDataFromTag(ItemTag).ItemAbilityClass,1);
	GetHASC()->GiveAbilityAndActivateOnce(AbilitySpec);
}

void AHPlayerState::AddKillCount()
{
	KillCount ++;
	OnKillCountChangedDelegate.Broadcast(KillCount);
}

void AHPlayerState::OnRep_KillCount()
{
	OnKillCountChangedDelegate.Broadcast(KillCount);
}

void AHPlayerState::AddDeathCount()
{
	DeathCount ++;
	OnDeathCountChangedDelegate.Broadcast(DeathCount);
}

void AHPlayerState::OnRep_DeathCount()
{
	OnDeathCountChangedDelegate.Broadcast(DeathCount);
}

void AHPlayerState::AddLastHitCount()
{
	LastHitCount ++;
	OnLastHitCountChangedDelegate.Broadcast(LastHitCount);
}

void AHPlayerState::OnRep_LastHitCount()
{
	OnLastHitCountChangedDelegate.Broadcast(LastHitCount);
}

void AHPlayerState::Server_SetHeroTag_Implementation(FGameplayTag tag)
{
	HeroTag = tag;
}

void AHPlayerState::Server_SetIsRadiantPlayer_Implementation(bool bRadiant)
{
	bIsRadiant = bRadiant;
}




