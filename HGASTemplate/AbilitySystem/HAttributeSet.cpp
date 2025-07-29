// Fill out your copyright notice in the Description page of Project Settings.


#include "HAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "HAbilitySystemComponent.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "HAbilityTypes/HAbilityTypes.h"
#include "HGASTemplate/Characters/HBaseCharacter.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/Interfaces/HGameModeInterface.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"


struct FHGameplayEffectContext;

UHAttributeSet::UHAttributeSet()
{
	const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();

	// Burda teker teker ekleyip map oluşturmak zorundayız ama mapi oşutduktan sonra widgetcontrollerda for loopa sokabiliyoruz
	
	TagsToAttributes.Add(GameplayTags.Attributes_MaxHealth,GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_HealthRegen,GetHealthRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_MaxMana,GetMaxManaAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_ManaRegen,GetManaRegenAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_AttackDamage,GetAttackDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_AttackSpeed,GetAttackSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_AbilityPower,GetAbilityPowerAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_AttackRange,GetAttackRangeAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_PhysicalArmor,GetPhysicalArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_MagicalArmor,GetMagicalArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_CriticalHitDamageCoefficient,GetCriticalHitDamageCoefficientAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_LifeSteal,GetLifeStealAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_CooldownReduction,GetCooldownReductionAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_PhysicalArmorPenetration,GetPhysicalArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_MagicalArmorPenetration,GetMagicalArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_MoveSpeed,GetMoveSpeedAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_BashChance,GetBashChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_NormalAttackSlow,GetNormalAttackSlowAttribute);

	/* Vital Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Health,GetHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Mana,GetManaAttribute);
	
}

void UHAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,HealthRegen,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,ManaRegen,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,AttackDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,AttackSpeed,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,AbilityPower,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,AttackRange,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,PhysicalArmor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,MagicalArmor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,CriticalHitDamageCoefficient,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,LifeSteal,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,CooldownReduction,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,PhysicalArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,MagicalArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,MoveSpeed,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,BashChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,NormalAttackSlow,COND_None,REPNOTIFY_Always);

	// Vital
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UHAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	
}

/**
 * But remember that all this does is clamp what's returned from querying the modifier.
 * Anything else that queries the modifier again is going to recalculate what's returned from the modifier of any given effect.
 * We are going to actually set the value to a clamped value here in post gameplay effect execute.
 */
void UHAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) 
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxHealth());
		/**This just changes the value returned from querying the modifier.*/
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0,GetMaxMana());
		/**This just changes the value returned from querying the modifier.*/
	}
}

// Attr. lerin değişiminin uygulandığı son fonk burası. Clamp için en ideal yer
void UHAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data,Props);
	
	if(Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;
	
	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0,GetMaxHealth()));
	}
	
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0,GetMaxMana()));
	}

	DebuffControl(Props);

	if (Data.EvaluatedData.Attribute == GetIncomingHealAttribute())
	{
		HandleIncomingHeal(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingLifeStealAttribute())
	{
		HandleIncomingLifeSteal(Props);
	}
	
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute()) // Serverde gerçekleşti çünkü incomingdmgattr replicated değil
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UHAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	/*
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealthRegen(GetMaxHealth());
		bTopOffHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}*/
}

void UHAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBasedEffect, adding to IncomingXP
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UHAttributeSet::HandleIncomingHeal(const FEffectProperties& Props)
{
	const float LocalIncomingHeal = GetIncomingHeal();
	SetIncomingHeal(0.f);
	
	if (LocalIncomingHeal > 0.f)
	{
		if (Props.TargetAvatarActor->Implements<UCombatInterface>())
		{
			const float NewHealth = GetHealth() + LocalIncomingHeal;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		}
		
		ShowFloatingTextHeal(Props, -LocalIncomingHeal, false, false); // - olarak gönderilmeli
	}
}

void UHAttributeSet::HandleIncomingLifeSteal(const FEffectProperties& Props)
{
	const float LocalIncomingLifeSteal = GetIncomingLifeSteal();
	SetIncomingLifeSteal(0.f);
	
	if (LocalIncomingLifeSteal > 0.f)
	{
		// Source Charaacterin AS sine ulaşıp Healti arttırmalıyız. Şu an Targetin AS sindeyiz
		if (Props.SourceAvatarActor->Implements<UCombatInterface>() && ICombatInterface::Execute_I_GetHAS(Props.SourceAvatarActor))
		{
			const float NewHealth = ICombatInterface::Execute_I_GetHAS(Props.SourceAvatarActor)->GetHealth()  + LocalIncomingLifeSteal;
			float SourceMaxHealth = ICombatInterface::Execute_I_GetHAS(Props.SourceAvatarActor)->GetMaxHealth();
			ICombatInterface::Execute_I_GetHAS(Props.SourceAvatarActor)->SetHealth(FMath::Clamp(NewHealth, 0.f, SourceMaxHealth));
		}
		ShowFloatingText(Props, -LocalIncomingLifeSteal, false, false); // - olarak gönderilmeli
	}
}

void UHAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage > 0.f)
	{
		
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			SendXPEvent(Props);

			AddGoldToKiller(Props);

			
			if (UGameplayStatics::GetGameMode(this))
			{
				if (IHGameModeInterface* HGinterface = Cast<IHGameModeInterface>(UGameplayStatics::GetGameMode(this)))
				{
					HGinterface->I_KillEvent(Props.SourceAvatarActor,Props.TargetAvatarActor);
				}
			}
			else
			{
				HLOG("GameModeÇekme başarısız");
			}
			
			if (ICombatInterface* DC = Cast<ICombatInterface>(Props.TargetCharacter))
			{
				DC->Die(FVector::ZeroVector, Props.SourceAvatarActor);
			}
		}

		CheckForJungleMob(Props);

		CheckForTower(Props);

		if (ShouldShowDamageIndicator(Props))
		{
			const bool bBlock = UHBlueprintFunctionLibrary::IsBlockedHit(Props.EffectContextHandle);
			const bool bCriticalHit = UHBlueprintFunctionLibrary::GetIsCriticalHit(Props.EffectContextHandle);
		
			ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		}

		if (Props.TargetAvatarActor->ActorHasTag(FName("Player"))) // Player Characterlere hasar aldığının bilgisi gönderiliyor, bazı GA lar buna göre işlem yapacak
		{
			FGameplayEventData Data;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.TargetAvatarActor,FHGamePlayTags::Get().Event_Attacked,Data);
		}
		
		/*if (UHBlueprintFunctionLibrary::GetIsDebuff(Props.EffectContextHandle))
		{
			Debuff(Props);
		}*/
	}
	
}

void UHAttributeSet::DebuffControl(const FEffectProperties& Props)
{
	// Source = Causer off the effect    Target = target of the effect (owner of this AS)
	// Criticali Hit Ve Bash ı Debufflardan ayrı Tutalım

	const FHGamePlayTags& Tags = FHGamePlayTags::Get();

	const FGameplayEffectContextHandle* Context = &Props.EffectContextHandle;

	UHAbilitySystemComponent* TargetASC = Cast<UHAbilitySystemComponent>(Props.TargetASC);
	if (!IsValid(TargetASC)) return;
	
	if (UHBlueprintFunctionLibrary::GetIsBashHit(*Context))
	{
		TargetASC->CreateTimedTagEffectSpecAndApply(Tags.Debuff_Stun,1.5f);
	}

	if (UHBlueprintFunctionLibrary::GetIsKnockBack(*Context))
	{
		UE_LOG(LogTemp,Error,TEXT("KnockBack"));
		
		TargetASC->CreateTimedTagEffectSpecAndApply(Tags.Debuff_Silence,1.f);
		if (ACharacter* CH = Cast<ACharacter>(TargetASC->GetAvatarActor()))
		{
			const FVector& V = UHBlueprintFunctionLibrary::GetKnockbackForce(*Context);
			CH->LaunchCharacter(V,true,true);
		}
	}

	// Geri kalan Debuffları GetIsDebuff ile kontrol edip uygulayalım.
	/**
	 * Knockback
	 * Silence
	 * Stun
	 * Slow
	 * Burn
	 */

	/*bool bIsDebufff = UHBlueprintFunctionLibrary::GetIsDebuff(*Context);
	if (bIsDebufff == false) return;*/
	
}

// C++ tan GE oluşturunca replike olmuyor

// EffectContextHandle dan Bilgileri çekip, 1 Adet Debuff Type Uygulaybiliyoruz. Bu bize pek bir Esneklik katmıyor.
// Daha çok damage veren 1 adet Debuffta Etkili


void UHAttributeSet::Debuff(const FEffectProperties& Props) 
{
	const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DebuffTypeTag = UHBlueprintFunctionLibrary::GetDebuffTypeTag(Props.EffectContextHandle);
	const FGameplayTag DamageType = UHBlueprintFunctionLibrary::GetDamageTypeTag(Props.EffectContextHandle);
	const float DebuffDamage = UHBlueprintFunctionLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UHBlueprintFunctionLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UHBlueprintFunctionLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	
	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DebuffTypeTag.ToString());
	// GameplayEffect Bp class olmadan C++ tan oluşturma işlemi
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	
	FInheritedTagContainer TagContainer;
	TagContainer.AddTag(DebuffTypeTag);
	auto& Comp = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	Comp.SetAndApplyTargetTagChanges(TagContainer);
	
	//Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);

	// Stephenın Tagi ekleme şekli
	//UTargetTagsGameplayEffectComponent& AssetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	//FInheritedTagContainer InheritedTagContainer;
	//InheritedTagContainer.Added.AddTag(DebuffTag); // Added ? 

	if (DebuffTypeTag.MatchesTagExact(GameplayTags.Debuff_Stun))
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
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UHAttributeSet::GetIncomingDamageAttribute();

	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FHGameplayEffectContext* HContext = static_cast<FHGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		HContext->SetDamageTypeTag(DebuffDamageType);
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UHAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	// Source = Causer off the effect    Target = target of the effect (owner of this AS)
	
	Props.EffectContextHandle =Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetInstigatorAbilitySystemComponent(); // Source ASC

	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		
		if(Props.SourceController == nullptr && Props.SourceAvatarActor)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UHAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		/*const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter); // BP native event olduğu için Execute_
		const int32 XPReward = UHBlueprintFunctionLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);*/

		// Ölünce 1600 birimlik alanda bulunan düşman playerlara exp gönderilmeli

		TArray<AActor*> OverlapActors;
		TArray<AActor*> ActorsToIgnore;
		float Radius = 1600; // Lolden alındı
		FVector SphereOrigin = Props.TargetCharacter->GetActorLocation();
		
		UHBlueprintFunctionLibrary::GetLivePlayersWithinRadius(this,OverlapActors,ActorsToIgnore,Radius,SphereOrigin);

		if(OverlapActors.Num() > 0 ) // Alanda istenilen actorler varsa
		{
			TArray<AActor*> ActorsForExp;
			for (auto& OverlapActor : OverlapActors)
			{
				if(OverlapActor->ActorHasTag(FName("Player")) && UHBlueprintFunctionLibrary::IsEnemy(OverlapActor,Props.TargetCharacter))
				{
					UE_LOG(LogTemp,Error,TEXT("Actors For Exp e karakter eklendi"));
					ActorsForExp.AddUnique(OverlapActor);
				}
			}

			if(ActorsForExp.Num()>0)
			{
				float ExpReward = ICombatInterface::Execute_GetXPReward(Props.TargetCharacter);
				float Exp = ExpReward / ActorsForExp.Num();
				
				const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();
				
				for (auto& ExpActor : ActorsForExp)
				{
					FGameplayEventData Payload;
					Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
					Payload.EventMagnitude = Exp;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(ExpActor, GameplayTags.Attributes_Meta_IncomingXP, Payload);
				}
			}
		}
	}
}

// Targetten Goldu AL
// Goldu Killera gönder
// Killer makinesinde victim üstünde gold oynat


void UHAttributeSet::AddGoldToKiller(const FEffectProperties& Props)
{
	if (Props.SourceAvatarActor && Props.TargetAvatarActor && Props.SourceAvatarActor!= Props.TargetAvatarActor)
	{
		if (Props.SourceAvatarActor->ActorHasTag(FName("Player"))) // Killer Player Characterdir
		{

			AHBaseCharacter* SourceCharacter = Cast<AHBaseCharacter>(Props.SourceAvatarActor);
			AHBaseCharacter* TargetCharacter = Cast<AHBaseCharacter>(Props.TargetAvatarActor);

			APlayerState* SourcePlayerState = SourceCharacter->GetPlayerState();
			if (!IsValid(SourcePlayerState)  || !SourcePlayerState->Implements<UPlayerInterface>()) return;

			if (TargetCharacter->Implements<UCombatInterface>())
			{
				int32 GoldToAdd = ICombatInterface::Execute_GetGoldReward(TargetCharacter);
				IPlayerInterface::Execute_AddToGold(SourcePlayerState,GoldToAdd);

				SourceCharacter->Client_ShowGoldNumber(GoldToAdd,TargetCharacter);
			}
			
		}
	}
}

void UHAttributeSet::CheckForJungleMob(const FEffectProperties& Props)
{
	if (Props.SourceAvatarActor && Props.TargetAvatarActor && Props.SourceAvatarActor!= Props.TargetAvatarActor &&
		Props.SourceAvatarActor->ActorHasTag(FName("Player")) &&
		Props.TargetAvatarActor->ActorHasTag(FName("JungleMob")))
	{
		TArray<AActor*> JungleActors;
		UHBlueprintFunctionLibrary::GetJungleLiveMobWithinRadius(Props.TargetAvatarActor,JungleActors,Props.SourceAvatarActor,600,Props.TargetAvatarActor->GetActorLocation());

		if(JungleActors.Num() > 0) // Çevredeki dost jungle moblar ve saldırılana bilgi gidiyor
		{
			for (auto& JungleActor : JungleActors)
			{
				ICombatInterface::Execute_SetTargetActor(JungleActor,Props.SourceAvatarActor);
			}
		}
		
	}
}

void UHAttributeSet::CheckForTower(const FEffectProperties& Props)
{
	if (Props.SourceAvatarActor && Props.TargetAvatarActor && Props.SourceAvatarActor!= Props.TargetAvatarActor &&
		Props.SourceAvatarActor->ActorHasTag(FName("Player")) &&
		Props.TargetAvatarActor->ActorHasTag(FName("Player")))
	{
		FVector Origin = Props.SourceAvatarActor->GetActorLocation();
		AActor* Tower = UHBlueprintFunctionLibrary::GetTowerWithinRadius(Props.SourceAvatarActor,1200,Origin);

		if (Tower && UHBlueprintFunctionLibrary::IsEnemy(Tower,Props.SourceAvatarActor))
		{
			ICombatInterface::Execute_SetTargetActor(Tower,Props.SourceAvatarActor);
		}
	}
}

void UHAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Damage <= 0.f) // Heal geliyor
	{
		/*if (Props.SourceCharacter != Props.TargetCharacter)
		{
			
		}*/ //? Belki kendi kendine heal atıyor

		if(AHBaseCharacter* HC = Cast<AHBaseCharacter>(Props.SourceCharacter))
		{
			HC->Multicast_ShowDamageNumber(Damage, Props.SourceCharacter,bBlockedHit,bCriticalHit);
		}
		if(AHBaseCharacter* HC = Cast<AHBaseCharacter>(Props.TargetCharacter)) // H hasar yiyince Hnın yediği damaage görmekiçin
		{
			HC->Multicast_ShowDamageNumber(Damage, Props.SourceCharacter,bBlockedHit,bCriticalHit);
		}
	}
	else // Damage geliyor
	{
		if (Props.SourceCharacter != Props.TargetCharacter)
		{
			if(AHBaseCharacter* HC = Cast<AHBaseCharacter>(Props.SourceCharacter))
			{
				HC->Multicast_ShowDamageNumber(Damage, Props.TargetCharacter,bBlockedHit,bCriticalHit);
			}
			if(AHBaseCharacter* HC = Cast<AHBaseCharacter>(Props.TargetCharacter)) // H hasar yiyince Hnın yediği damaage görmekiçin
			{
				HC->Multicast_ShowDamageNumber(Damage, Props.TargetCharacter,bBlockedHit,bCriticalHit);
			}
		}
	}
}

void UHAttributeSet::ShowFloatingTextHeal(const FEffectProperties& Props, float Damage, bool bBlockedHit,
	bool bCriticalHit) const
{
	if (Damage <= 0.f) // Heal geliyor
	{

		if(AHBaseCharacter* HC = Cast<AHBaseCharacter>(Props.TargetAvatarActor))
		{
			HC->Multicast_ShowDamageNumber(Damage, HC,bBlockedHit,bCriticalHit);
		}
		
	}
}

bool UHAttributeSet::ShouldShowDamageIndicator(const FEffectProperties& Props) const
{
	if (Props.SourceCharacter->ActorHasTag(FName("Minion"))) return false;

	return true;
}

void UHAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,Health,OldHealth); 
}

void UHAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,MaxHealth,OldMaxHealth); 
}

void UHAttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,HealthRegen,OldHealthRegen); 
}

void UHAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,MaxMana,OldMaxMana); 
}

void UHAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,Mana,OldMana); 
}

void UHAttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,ManaRegen,OldManaRegen); 
}

void UHAttributeSet::OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,AttackDamage,OldAttackDamage); 
}

void UHAttributeSet::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,AttackSpeed,OldAttackSpeed); 
}

void UHAttributeSet::OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,AbilityPower,OldAbilityPower); 
}

void UHAttributeSet::OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,AttackRange,OldAttackRange); 
}

void UHAttributeSet::OnRep_PhysicalArmor(const FGameplayAttributeData& OldPhysicalArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,PhysicalArmor,OldPhysicalArmor); 
}

void UHAttributeSet::OnRep_MagicalArmor(const FGameplayAttributeData& OldMagicalArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,MagicalArmor,OldMagicalArmor); 
}

void UHAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,CriticalHitChance,OldCriticalHitChance); 
}

void UHAttributeSet::OnRep_CriticalHitDamageCoefficient(const FGameplayAttributeData& OldCriticalHitDamageCoefficient) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,CriticalHitDamageCoefficient,OldCriticalHitDamageCoefficient); 
}

void UHAttributeSet::OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,LifeSteal,OldLifeSteal); 
}

void UHAttributeSet::OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,CooldownReduction,OldCooldownReduction); 
}

void UHAttributeSet::OnRep_PhysicalArmorPenetration(const FGameplayAttributeData& OldPhysicalArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,PhysicalArmorPenetration,OldPhysicalArmorPenetration); 
}

void UHAttributeSet::OnRep_MagicalArmorPenetration(const FGameplayAttributeData& OldMagicalArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,MagicalArmorPenetration,OldMagicalArmorPenetration); 
}

void UHAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,MoveSpeed,OldMoveSpeed);
}

void UHAttributeSet::OnRep_BashChance(const FGameplayAttributeData& OldBashChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,BashChance,OldBashChance);
}

void UHAttributeSet::OnRep_NormalAttackSlow(const FGameplayAttributeData& OldNormalAttackSlow) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UHAttributeSet,NormalAttackSlow,OldNormalAttackSlow);
}