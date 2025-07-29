// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlayWidgetController.h"

#include "MarketWidgetController.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Data/Data_Hero.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GameCore/HGameInstance.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"

UOverlayWidgetController::UOverlayWidgetController()
{
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	UHAbilitySystemComponent* ASC = GetHASC();

	/*for (auto& Pair : GetHAS()->TagsToAttributes)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				AttributeChangedDelegate.Broadcast(Pair.Key,Data.NewValue);
			});
	}
	
	if(GetHPS()) // Player Controlled ve Server Controlled Karakterlerin bilgisi
	{
		GetHPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 Val,bool bLevelUpp)
		{
			LevelChangedDelegate.Broadcast(Val);
		});

		GetHPS()->OnSpellPointsChangedDelegate.AddLambda(
			[this](int32 Val)
			{
				SpellPointDelegate.Broadcast(Val);
			});

		GetHPS()->UpgradeButtonDelegate.AddLambda(
			[this](TArray<FSkillUpgradeInfo> UpgradeInfos)
			{
				UpgradeButtonsWorks(UpgradeInfos);
			});

		GetHPS()->UpgradeSkillInfoDelegate.AddLambda(
			[this](const FUpgradeSkillInfoForOverlay& UpgradeSkillInfo)
			{
				SkillUnlockedDelegate.Broadcast(UpgradeSkillInfo);
			});

		GetHPS()->GoldChangedDelegate.AddLambda(
			[this](int32 Gold)
			{
				GoldChangedDelegate.Broadcast(Gold);
			});

		GetHPS()->OnInventoryItemStructDelegate.AddLambda(
			[this](const FInventoryItemStruct& ItemStruct)
			{
				OnInventoryItemStructDelegate.Broadcast(ItemStruct);
			});

		GetHPS()->OnKillCountChangedDelegate.AddLambda(
			[this](int32 Value)
			{
				OnKillCountChangedDelegate.Broadcast(Value);
			});

		GetHPS()->OnDeathCountChangedDelegate.AddLambda(
			[this](int32 Value)
			{
				OnDeathCountChangedDelegate.Broadcast(Value);
			});

		GetHPS()->OnLastHitCountChangedDelegate.AddLambda(
			[this](int32 Value)
			{
				OnLastHitCountChangedDelegate.Broadcast(Value);
			});

		GetHPS()->OnGameTimeChangedDelegate.AddLambda(
			[this](float Value)
			{
				OnGameTimeChangedDelegate.Broadcast(Value);
			});
		
	}
	
	else if(AbilitySystemComponent->GetAvatarActor()) // Sim Proxyler için Level bilgisi
	{
		if(ICombatInterface* ic = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
		{
			ic->GetLevelChangedDelegate().AddLambda(
				[this](float Value)
				{
					LevelChangedDelegate.Broadcast(Value);
				});
		}
	}*/
	
	for (auto& Pair : GetHAS()->TagsToAttributes)
    	{
    		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddWeakLambda(this,
    			[this,Pair](const FOnAttributeChangeData& Data)
    			{
    				AttributeChangedDelegate.Broadcast(Pair.Key,Data.NewValue);
    			});
    	}
    	
    	if(GetHPS()) // Player Controlled ve Server Controlled Karakterlerin bilgisi
    	{
    		GetHPS()->OnLevelChangedDelegate.AddWeakLambda(this,
    		[this](int32 Val,bool bLevelUpp)
    		{
    			LevelChangedDelegate.Broadcast(Val);
    		});
    
    		GetHPS()->OnSpellPointsChangedDelegate.AddWeakLambda(this,
    			[this](int32 Val)
    			{
    				SpellPointDelegate.Broadcast(Val);
    			});
    
    		GetHPS()->UpgradeButtonDelegate.AddWeakLambda(this,
    			[this](TArray<FSkillUpgradeInfo> UpgradeInfos)
    			{
    				UpgradeButtonsWorks(UpgradeInfos);
    			});
    
    		GetHPS()->UpgradeSkillInfoDelegate.AddWeakLambda(this,
    			[this](const FUpgradeSkillInfoForOverlay& UpgradeSkillInfo)
    			{
    				SkillUnlockedDelegate.Broadcast(UpgradeSkillInfo);
    			});
    
    		GetHPS()->GoldChangedDelegate.AddWeakLambda(this,
    			[this](int32 Gold)
    			{
    				GoldChangedDelegate.Broadcast(Gold);
    			});
    
    		GetHPS()->OnInventoryItemStructDelegate.AddWeakLambda(this,
    			[this](const FInventoryItemStruct& ItemStruct)
    			{
    				OnInventoryItemStructDelegate.Broadcast(ItemStruct);
    			});
    
    		GetHPS()->OnKillCountChangedDelegate.AddWeakLambda(this,
    			[this](int32 Value)
    			{
    				OnKillCountChangedDelegate.Broadcast(Value);
    			});
    
    		GetHPS()->OnDeathCountChangedDelegate.AddWeakLambda(this,
    			[this](int32 Value)
    			{
    				OnDeathCountChangedDelegate.Broadcast(Value);
    			});
    
    		GetHPS()->OnLastHitCountChangedDelegate.AddWeakLambda(this,
    			[this](int32 Value)
    			{
    				OnLastHitCountChangedDelegate.Broadcast(Value);
    			});
    
    		GetHPS()->OnGameTimeChangedDelegate.AddWeakLambda(this,
    			[this](float Value)
    			{
    				OnGameTimeChangedDelegate.Broadcast(Value);
    			});
    		
    	}
    	
    	else if(AbilitySystemComponent->GetAvatarActor()) // Sim Proxyler için Level bilgisi
    	{
    		if(ICombatInterface* ic = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
    		{
    			ic->GetLevelChangedDelegate().AddWeakLambda(this,
    				[this](float Value)
    				{
    					LevelChangedDelegate.Broadcast(Value);
    				});
    		}
    	}
	
}

FHeroDataStruct& UOverlayWidgetController::GetHeroData() const
{
	check(AbilitySystemComponent);
	check(AbilitySystemComponent->GetAvatarActor());
	FGameplayTag HeroTag = ICombatInterface::Execute_GetHeroTag(AbilitySystemComponent->GetAvatarActor());

	UHGameInstance * HI = GetWorld()->GetGameInstance<UHGameInstance>();
	check(HI);

	return HI->HeroAssetData.Get()->GetHeroDataByHeroTag(HeroTag);
}

void UOverlayWidgetController::InitializeUpgradeButtonDelegateMap()
{
	HLOG("Upgrade button delegate map init oldu");
	FHGamePlayTags Tags = FHGamePlayTags::Get();
	UpgradeButtonDelegateMap.Add(Tags.InputTag_Q,UpgradeButtonQDelegate);
	UpgradeButtonDelegateMap.Add(Tags.InputTag_W,UpgradeButtonWDelegate);
	UpgradeButtonDelegateMap.Add(Tags.InputTag_E,UpgradeButtonEDelegate);
	UpgradeButtonDelegateMap.Add(Tags.InputTag_R,UpgradeButtonRDelegate);
}

void UOverlayWidgetController::UpgradeButtonClicked(const FGameplayTag& SkillTag)
{
	GetHPS()->Server_UpgradeSkill(SkillTag);
}

void UOverlayWidgetController::UpgradeButtonsWorks(const TArray<FSkillUpgradeInfo>& UpgradeInfos)
{
	/*for (auto& Element : UpgradeInfos)
	{
		UpgradeButtonDelegateMap.FindChecked(Element.SkillTag).Broadcast(Element.bActivate);
	}*/
	for (const auto& Element : UpgradeInfos)
	{
		if (auto* DelegatePtr = UpgradeButtonDelegateMap.Find(Element.SkillTag))
		{
			DelegatePtr->Broadcast(Element.bActivate);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SkillTag %s not found in UpgradeButtonDelegateMap"), *Element.SkillTag.ToString());
		}
	}
}

UMarketWidgetController* UOverlayWidgetController::MakeMarketWidgetControllerWithPSPC()
{
	UMarketWidgetController* MarketController = NewObject<UMarketWidgetController>(this,MarketWidgetControllerClass);
	if (MarketController)
	{
		MarketController->HPlayerState = GetHPS();
		MarketController->HPlayerController = GetHPC();
	}
	return MarketController;
}

void UOverlayWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetHAS()->TagsToAttributes)
	{
		AttributeChangedDelegate.Broadcast(Pair.Key,Pair.Value().GetNumericValue(GetHAS()));
	}
	
	if(GetHPS() == nullptr) return; // Tembellik edip Overlay Widget Controlleru Bar componenttle minyonlardada kullandığımız için ???
	
	LevelChangedDelegate.Broadcast(GetHPS()->GetPlayerLevel());
	SpellPointDelegate.Broadcast(GetHPS()->GetSpellPoints());
	GoldChangedDelegate.Broadcast(GetHPS()->Gold);
}


