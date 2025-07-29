// Fill out your copyright notice in the Description page of Project Settings.

#include "HAbilitySystemComponent.h"

#include <functional>

#include "AbilitySystemBlueprintLibrary.h"
#include "AudioMixerBlueprintLibrary.h"
#include "HAttributeSet.h"
#include "NiagaraValidationRule.h"
#include "Abilities/HGameplayAbility.h"
#include "Abilities/HPassiveGameplayAbility.h"
#include "Data/DynamicSkillInfoObject.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

UHAbilitySystemComponent::UHAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void UHAbilitySystemComponent::I_CancelAllActionAbilities_Implementation()
{
	CancelAllActionAbilities();
}

float UHAbilitySystemComponent::I_GetAttributeValueByTag_Implementation(const FGameplayTag& AttributeTag)
{
	return GetAttributeValueByTag(AttributeTag);
}

void UHAbilitySystemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UHAbilitySystemComponent::AbilityActorInfoSet()
{
	if (GetAvatarActor()->HasAuthority())
	{
		RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				CancelAllActionAbilities();
			}
		});

		RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Knockback, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
			[this](const FGameplayTag& CallBackTag, int32 NewCount)
			{
				if (NewCount > 0)
				{
					CancelAllActionAbilities();
				}
			});
	}
}

void UHAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for(auto& AbilityClass: StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1); // Ability classla AbilitySpec oluşturuldu

		if(const UHGameplayAbility* HAbility = Cast<UHGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(HAbility->SkillInputTag);
			GiveAbility(AbilitySpec); // Ability, ASC ye eklenmiş oluyor

			if (HAbility->DynamicSkillInfoObjectClass)
			{
				Client_InitializeClientDynamicInfoObject(HAbility->DynamicSkillInfoObjectClass);
			}
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UHAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{

	//PassiveAbilitiesClassArray = StartupPassiveAbilities; ?

	if(StartupPassiveAbilities.Num() == 0) return;

	for(auto& AbilityClass: StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1); // Ability classla AbilitySpec oluşturuldu

		if(const UHGameplayAbility* HAbility = Cast<UHGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(HAbility->SkillInputTag);

			if (const UHPassiveGameplayAbility* PA = Cast<UHPassiveGameplayAbility>(AbilitySpec.Ability)) // Karakter pasifi ise
			{
				if (PA->bPassiveSlotAbility == false && PA->DynamicSkillInfoObjectClass) // Karakter pasifi ise
				{
					Client_InitializeClientDynamicInfoObject(PA->DynamicSkillInfoObjectClass);
					UE_LOG(LogTemp, Warning, TEXT("Karakter pasifi için dynamicinfo object oluşturuldu"));
				}
			}
			
			GiveAbilityAndActivateOnce(AbilitySpec); // Ability, ASC ye eklenmiş oluyor

			// Spec verildikten sonra kontrol
			/*FGameplayAbilitySpec* GivenSpec = FindAbilitySpecFromClass(AbilityClass);
			if (GivenSpec)
			{
				UE_LOG(LogTemp, Log, TEXT("Ability spec başarıyla eklendi"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Ability spec eklenemedi"));
			}*/
		}
		
		else // HAbility olmasına gerek olmayan Basit Abilityler için
		{
			GiveAbilityAndActivateOnce(AbilitySpec);
		}
		
	}
}

void UHAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(FHGamePlayTags::Get().InputTag_Block)) return;
			
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UHAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLoc(*this);

	for(auto& AbilitySpec : GetActivatableAbilities()) // GetActivatableAbilities() ->  Anladığımıza göre GiveAbility(AbilitySpec); bununla ekledik activatable abilitylere
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(FHGamePlayTags::Get().InputTag_Block)) return;
		
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec); // abilitye presslendiğinin bilgisini iletiyoruz
			
			if(!AbilitySpec.IsActive()) // Ability aktif değilse devam et
			{
				if (IsAbilitySkill(InputTag))
				{
					if (AbilitySpec.DynamicAbilityTags.HasTagExact(FHGamePlayTags::Get().Status_Unlocked))
					{
						TryActivateAbility(AbilitySpec.Handle);
					}
				}
				else
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}

void UHAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLoc(*this);

	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec); 
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UHAbilitySystemComponent::ActivateNormalAttackAbility()
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.HasTagExact(FHGamePlayTags::Get().NormalAttack) && !AbilitySpec.IsActive())
		{
			TryActivateAbility(AbilitySpec.Handle);
			return;
		}
	}
	
}

void UHAbilitySystemComponent::CancelAllActionAbilities()
{
	if (GetActivatableAbilities().Num() == 0) return;
	
	FScopedAbilityListLock ActiveScopeLoc(*this);

	FHGamePlayTags Tags = FHGamePlayTags::Get();
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.IsActive() && AbilitySpec.Ability->AbilityTags.HasTagExact(Tags.AbilityTypes_Action))
		{
			CancelAbility(AbilitySpec.Ability);
		}
	}
}

void UHAbilitySystemComponent::EndNormalAttackAbility()
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.HasTagExact(FHGamePlayTags::Get().NormalAttack) && AbilitySpec.IsActive())
		{
			CancelAbility(AbilitySpec.Ability);
		}
	}
	
}

UGameplayAbility* UHAbilitySystemComponent::GetNormalAttackAbility()
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	for(auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.Ability->AbilityTags.HasTagExact(FHGamePlayTags::Get().NormalAttack))
		{
			return AbilitySpec.Ability;
		}
	}
	return nullptr;
}

void UHAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this); // Used to stop us from removing abilities from an ability system component while we're iterating through the abilities
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UHAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UHAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UHAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UHAbilitySystemComponent::GetAbilitySpecFromTag(const FGameplayTag& AbilityTag)
{
	if (GetActivatableAbilities().Num() == 0) return nullptr;
	
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	
	return nullptr;
}


UHGameplayAbility* UHAbilitySystemComponent::GetHAbilityFromTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(AbilityTag))
		{
			return Cast<UHGameplayAbility>(AbilitySpec.Ability.Get());
		}
	}
	return nullptr;
}

UHGameplayAbility* UHAbilitySystemComponent::GetSkillAbilityFromTag(const FGameplayTag& SkillTag)
{
	if (IsAbilitySkill(SkillTag))
	{
		return GetHAbilityFromTag(SkillTag);
	}
	return nullptr;
}

void UHAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag,
                                                                 const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

void UHAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if(!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UHAbilitySystemComponent::Client_EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                   const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTagsDelegate.Broadcast(TagContainer); // widget controllerda var bind fonklar
}

void UHAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

FGameplayAbilitySpec* UHAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

float UHAbilitySystemComponent::GetAttributeValueByTag(const FGameplayTag AttributeTag) const
{
	if (const UHAttributeSet* AttributeSet = Cast<UHAttributeSet>(GetAttributeSet(UHAttributeSet::StaticClass())))
	{
		for (auto& Element : AttributeSet->TagsToAttributes)
		{
			if (AttributeTag.MatchesTagExact(Element.Key))
			{
				return Element.Value().GetNumericValue(GetAttributeSet(UHAttributeSet::StaticClass()));
			}
		}
	}
	return 0.f;
}

void UHAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UHAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	// Önemli yer hnagi tage sahip attributeye işlem uygulanacağı bilgisi GA_Listen a gidiyor 
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	// Attribute arttırıldığında, attribute pointsin eksiltilmesi gerekiyor
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

bool UHAbilitySystemComponent::IsSkillUnlocked(const FGameplayTag& SkillTag)
{
	//if (AbilityStatusArray.Num() == 0) return false;
	
	if (GetSkillAbilityFromTag(SkillTag) == nullptr) return false;

	return GetSkillAbilityFromTag(SkillTag)->AbilityTags.HasTagExact(FHGamePlayTags::Get().Status_Unlocked);
}

bool UHAbilitySystemComponent::IsAbilitySkill(const FGameplayTag& AbilityTag)
{
	FHGamePlayTags Tags = FHGamePlayTags::Get();
	if (AbilityTag.MatchesTagExact(Tags.InputTag_Q) || AbilityTag.MatchesTagExact(Tags.InputTag_W)
		|| AbilityTag.MatchesTagExact(Tags.InputTag_E) || AbilityTag.MatchesTagExact(Tags.InputTag_R))
	{
		return true;
	}
	return false;
}

void UHAbilitySystemComponent::UpgradeSkillLevel(const FGameplayTag& SkillTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	// Sadece server'da değişiklik yapabilir
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (!IsAbilitySkill(SkillTag)) return;
	if (GetSkillAbilityFromTag(SkillTag) == nullptr) return;

	FGameplayAbilitySpec* Spec = nullptr;
	FHGamePlayTags Tags = FHGamePlayTags::Get();
	bool bFoundSpec = false;

	for (auto& Element : GetActivatableAbilities())
	{
		if (Element.DynamicAbilityTags.HasTagExact(SkillTag))
		{
			Spec = &Element;
			bFoundSpec = true;
			break;
		}
	}

	if (!bFoundSpec)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill spec not found for tag: %s"), *SkillTag.ToString());
		return;
	}

	if (!Spec->DynamicAbilityTags.HasTagExact(Tags.Status_Unlocked))// Skill yeni açılıyorsa levelini arttırmıyoruz, çünkü leveli zaten 1
	{
		/*if (UHGameplayAbility* PA = Cast<UHGameplayAbility>(Spec->Ability.Get()))
		{
			
		}*/
			
		if (UHPassiveGameplayAbility* PA = Cast<UHPassiveGameplayAbility>(Spec->Ability.Get()))
		{
			if (PA->bPassiveSlotAbility)
			{
				TryActivateAbility(Spec->Handle); // Eğer pasif harici, Q W E R gibi slotlarda bulunan pasif bir yetenekse, onu aktif et çünkü input ile aktif edilmiyorlar
			}
		}

		Spec->DynamicAbilityTags.AddTag(Tags.Status_Unlocked);
		
		const FUpgradeSkillInfoForOverlay Info (SkillTag,1,true);
		Client_SendUpgradeSkillInfo(Info);
	}
	else
	{
		/*if (UHGameplayAbility* PA = Cast<UHGameplayAbility>(Spec->Ability.Get()))
		{
			
		}*/
		
		Spec->Level++;
		
		const FUpgradeSkillInfoForOverlay Info (SkillTag,Spec->Level,false);
		Client_SendUpgradeSkillInfo(Info);

		Client_UpdateDynamicSkillInfoObjectLevel(SkillTag,Spec->Level);

		if (UHPassiveGameplayAbility* PA = Cast<UHPassiveGameplayAbility>(Spec->Ability.Get()))
		{
			if (PA->bPassiveSlotAbility) // Pasif Skile kaç level olduğunun bilgisi veriliyor
			{
				FGameplayEventData Payload;
				Payload.EventMagnitude = Spec->Level;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),SkillTag, Payload);
			}
		}
	}

	MarkAbilitySpecDirty(*Spec);
}

void UHAbilitySystemComponent::UpgradeCharacterPassiveSkillLevel()
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	
	FHGamePlayTags Tags = FHGamePlayTags::Get();

	FGameplayAbilitySpec* Spec = nullptr;
	
	for (auto& Element : GetActivatableAbilities())
	{
		if (Element.DynamicAbilityTags.HasTagExact(Tags.InputTag_Passive))
		{
			Spec = &Element;
			
			if (Spec)
			{
				Spec->Level++;
				FGameplayEventData Payload;
				Payload.EventMagnitude = Spec->Level;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),Tags.InputTag_Passive, Payload);

				Client_UpdateDynamicSkillInfoObjectLevel(Tags.InputTag_Passive,Spec->Level);
			}
		}
	}
}

void UHAbilitySystemComponent::UpgradeSkillLevelByTag(const FGameplayTag SkillTag)
{
	// Sadece server'da değişiklik yapabilir
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	if (!IsAbilitySkill(SkillTag)) return;
	if (GetSkillAbilityFromTag(SkillTag) == nullptr) return;

	FGameplayAbilitySpec* Spec = nullptr;
	Spec = GetSpecFromAbilityTag(SkillTag);

	if (!Spec)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill spec not found for tag: %s"), *SkillTag.ToString());
		return;
	}

	Spec->Level++;
	MarkAbilitySpecDirty(*Spec);
		
	const FUpgradeSkillInfoForOverlay Info (SkillTag,Spec->Level,false);
	Client_SendUpgradeSkillInfo(Info);

	Client_UpdateDynamicSkillInfoObjectLevel(SkillTag,Spec->Level);
	
}

void UHAbilitySystemComponent::Client_SendUpgradeSkillInfo_Implementation(const FUpgradeSkillInfoForOverlay& Info)
{
	if (GetAvatarActor())
	{
		if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
		{
			if (Character->GetPlayerState<APlayerState>())
			{
				UObject* Obj = Cast<UObject>(Character->GetPlayerState());
				IPlayerInterface::Execute_UpgradeSkillInfoSend(Obj,Info);
			}
		}
	}
}

FGameplayEffectSpec* UHAbilitySystemComponent::CreateTimedTagEffectSpec(const FGameplayTag& Tag, float EffectDuration)
{
	const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActor());

	const FGameplayTag DebuffTypeTag = Tag;
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DebuffTypeTag.ToString());
	// GameplayEffect Bp class olmadan C++ tan oluşturma işlemi
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->DurationMagnitude = FScalableFloat(EffectDuration);
	
	FInheritedTagContainer TagContainer;
	TagContainer.AddTag(DebuffTypeTag);
	auto& Comp = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	Comp.SetAndApplyTargetTagChanges(TagContainer);

	if (DebuffTypeTag.MatchesTagExact(GameplayTags.Debuff_Stun) )
	{
		// Stunned, so block input
		TagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	
	Comp.SetAndApplyTargetTagChanges(TagContainer);
	
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	const int32 Index = Effect->Modifiers.Num();

	return new FGameplayEffectSpec(Effect, EffectContext, 1.f);
	
}

void UHAbilitySystemComponent::CreateTimedTagEffectSpecAndApply(const FGameplayTag& Tag, float EffectDuration)
{
	FGameplayEffectSpec* Spec = CreateTimedTagEffectSpec(Tag, EffectDuration);
	ApplyGameplayEffectSpecToSelf(*Spec);
}

void UHAbilitySystemComponent::Client_InitializeClientDynamicInfoObject_Implementation(
	TSubclassOf<UDynamicSkillInfoObject> DynamicSkillInfoObjectClass)
{
	UDynamicSkillInfoObject* DynamicSkillInfoObject = NewObject<UDynamicSkillInfoObject>(this, DynamicSkillInfoObjectClass);
	
	DynamicSkillInfoObject->UpdateSkillInfoStruct(); // 1. başlama leveli için Struct BP de Set edilecek bu İmplementable eventtir
	DynamicSkillInfoObjectsArray.Add(DynamicSkillInfoObject);

	const UHAttributeSet* HAS = Cast<UHAttributeSet>(GetAttributeSet(UHAttributeSet::StaticClass()));

	DynamicSkillInfoObject->AttackDamage = HAS->GetAttackDamage();
	DynamicSkillInfoObject->AbilityPower = HAS->GetAbilityPower();
	DynamicSkillInfoObject->UpdateSkillInfoStruct();
	
	GetGameplayAttributeValueChangeDelegate(HAS->GetAttackDamageAttribute()).AddLambda(
	[DynamicSkillInfoObject](const FOnAttributeChangeData& Data)
		{
			DynamicSkillInfoObject->AttackDamage = Data.NewValue;
			DynamicSkillInfoObject->UpdateSkillInfoStruct();
		});

	GetGameplayAttributeValueChangeDelegate(HAS->GetAbilityPowerAttribute()).AddLambda(
	[DynamicSkillInfoObject](const FOnAttributeChangeData& Data)
		{
			DynamicSkillInfoObject->AbilityPower = Data.NewValue;
			DynamicSkillInfoObject->UpdateSkillInfoStruct();
		});
}

UDynamicSkillInfoObject* UHAbilitySystemComponent::GetDynamicSkillInfoObjectByTag(const FGameplayTag& SkillTag)
{
	if (DynamicSkillInfoObjectsArray.Num() == 0) return nullptr;

	for (auto& Element : DynamicSkillInfoObjectsArray)
	{
		if (Element->SkillTag.MatchesTagExact(SkillTag))
		{
			return Element;
		}
	}
	return nullptr;
}

FString UHAbilitySystemComponent::GetSkillDescriptionByTag(const FGameplayTag& SkillTag)
{
	if (GetDynamicSkillInfoObjectByTag(SkillTag))
	{
		return GetDynamicSkillInfoObjectByTag(SkillTag)->SkillDescription;
	}
	return FString();
}

FDynamicSkillInfo UHAbilitySystemComponent::GetDynamicSkillInfoByTag(const FGameplayTag& SkillTag)
{
	if (GetDynamicSkillInfoObjectByTag(SkillTag))
	{
		return GetDynamicSkillInfoObjectByTag(SkillTag)->DynamicSkillInfoStruct;
	}
	return FDynamicSkillInfo();
}

void UHAbilitySystemComponent::Client_UpdateDynamicSkillInfoObjectLevel_Implementation(const FGameplayTag& SkillTag,
                                                                                       int32 NewLevel)
{
	UDynamicSkillInfoObject* DynamicSkillInfoObject = GetDynamicSkillInfoObjectByTag(SkillTag);
	if (DynamicSkillInfoObject)
	{
		DynamicSkillInfoObject->CurrentSkillLevel = NewLevel;
		DynamicSkillInfoObject->UpdateSkillInfoStruct();
	}
}












