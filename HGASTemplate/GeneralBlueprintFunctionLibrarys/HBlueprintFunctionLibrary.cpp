// Fill out your copyright notice in the Description page of Project Settings.


#include "HBlueprintFunctionLibrary.h"


#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"
#include "HGASTemplate/AbilitySystem/Abilities/HGameplayAbility.h"
#include "HGASTemplate/AbilitySystem/HAbilityTypes/HAbilityTypes.h"
#include "HGASTemplate/GameCore/HGameInstance.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"
#include "HGASTemplate/UI/HUDS/HHUD.h"
#include "HGASTemplate/UI/WidgetControllers/HWidgetController.h"
#include "HGASTemplate/UI/WidgetControllers/MarketWidgetController.h"
#include "AbilitySystemInterface.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Characters/HBaseCharacter.h"
#include "HGASTemplate/Data/Data_Hero.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GameCore/GameModes/HGameMode.h"
#include "Kismet/GameplayStatics.h"


bool UHBlueprintFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AHHUD*& OutHHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutHHUD = Cast<AHHUD>(PC->GetHUD());
		if (OutHHUD)
		{
			AHPlayerState* PS = PC->GetPlayerState<AHPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UMarketWidgetController* UHBlueprintFunctionLibrary::MakeMarketWidgetController(UObject* OuterObject)
{
	UMarketWidgetController* MarketController = NewObject<UMarketWidgetController>(OuterObject);
	return MarketController;
}

UData_Item* UHBlueprintFunctionLibrary::GetItemDataAsset(const UObject* ContextObj)
{
	UGameInstance* Ins = UGameplayStatics::GetGameInstance(ContextObj);
	
	if (Ins && ContextObj)
	{
		UHGameInstance* GameIns = Cast<UHGameInstance>(UGameplayStatics::GetGameInstance(ContextObj));
		if (GameIns)
		{
			return GameIns->ItemAssetData;
		}
	}
	return nullptr;
}

void UHBlueprintFunctionLibrary::ApplyGamePlaySpecWithSetByCallerTagAndClass(
	TSubclassOf<UGameplayEffect> InGameplayEffectClass, const FGameplayTag InSetByCallerTag, float Magnitude,
	AActor*EffectCauserActor, AActor* TargetActor)
{
	check(EffectCauserActor);
	check(TargetActor);

	if (EffectCauserActor == TargetActor) return;
	
	if (IsEnemy(EffectCauserActor,TargetActor))
	{
		UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectCauserActor);
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(SourceASC->GetAvatarActor());
		FGameplayEffectSpecHandle Handle = TargetASC->MakeOutgoingSpec(InGameplayEffectClass,1,EffectContextHandle);

		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(Handle,InSetByCallerTag,Magnitude);

		TargetASC->ApplyGameplayEffectSpecToSelf(*Handle.Data);
	}
	
}

FGameplayEffectContextHandle UHBlueprintFunctionLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	
	SetKnockbackForce(EffectContexthandle, DamageEffectParams.KnockbackForce);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageTypeTag, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
}

float UHBlueprintFunctionLibrary::GetAttributeValueByTag(AActor* OwnerActor,
                                                         const FGameplayTag AttributeTag)
{
	if (OwnerActor == nullptr) return 0.f;

	if (UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor))
	{
		UHAbilitySystemComponent* HASC = Cast<UHAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor));
		if (HASC)
		{
			return HASC->GetAttributeValueByTag(AttributeTag);
		}
		return 0;
	}

	return 0;
	
}

float UHBlueprintFunctionLibrary::GetValueFromScalableFloat(const FScalableFloat ScalableFloat, int32 Level)
{
	return ScalableFloat.GetValueAtLevel(Level);
}

UTexture2D* UHBlueprintFunctionLibrary::GetHeroIcon(APlayerController* PC, const FGameplayTag HeroTag)
{
	if (PC == nullptr) return nullptr;

	UHGameInstance * HI = PC->GetGameInstance<UHGameInstance>();
	check(HI);

	return HI->HeroAssetData.Get()->GetHeroIconByHeroTag(HeroTag);
}

FVector UHBlueprintFunctionLibrary::GetRecallLocation(const ACharacter* RecallCharacter, bool bIsRadiant,
	FRotator& OutRotator)
{
	if (RecallCharacter == nullptr)
	{
		OutRotator = FRotator::ZeroRotator;
		return FVector::ZeroVector;
	}

	AHGameModeBase* GM =Cast<AHGameModeBase>( UGameplayStatics::GetGameMode(RecallCharacter));
	check(GM);

	AActor* PStart = GM->FindHPlayerStartForTeam(bIsRadiant);

	OutRotator = PStart->GetActorRotation();

	return PStart->GetActorLocation();
}

void UHBlueprintFunctionLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams,
                                                          UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

bool UHBlueprintFunctionLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->IsBlockedHit();
	}
	return false;
}

bool UHBlueprintFunctionLibrary::GetIsDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->IsDebuff();
	}
	return false;
}

FGameplayTag UHBlueprintFunctionLibrary::GetDebuffTypeTag(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (HEffectContext->GetDebuffTypeTag().IsValid())
		{
			return *HEffectContext->GetDebuffTypeTag();
		}
	}
	return FGameplayTag();
}

float UHBlueprintFunctionLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UHBlueprintFunctionLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UHBlueprintFunctionLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UHBlueprintFunctionLibrary::GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (HEffectContext->GetDamageTypeTag().IsValid())
		{
			return *HEffectContext->GetDamageTypeTag();
		}
	}
	return FGameplayTag();
}

FVector UHBlueprintFunctionLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UHBlueprintFunctionLibrary::GetIsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->IsCriticalHit();
	}
	return false;
}

void UHBlueprintFunctionLibrary::SetIsDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsDebuff)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetIsDebuff(bInIsDebuff);
	}
}

void UHBlueprintFunctionLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UHBlueprintFunctionLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

bool UHBlueprintFunctionLibrary::GetIsBashHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->GetIsBashHit();
	}
	return false;
}

void UHBlueprintFunctionLibrary::SetIsBashHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBashHit)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetIsBashHit(bInIsBashHit);
	}
}

bool UHBlueprintFunctionLibrary::GetIsSlow(const FGameplayEffectContextHandle& EffectContextHandle)
{
	
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->bIsSlow;
	}
	return false;
}

void UHBlueprintFunctionLibrary::SetIsSlow(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSlow)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->bIsSlow = bInIsSlow;
	}
}

float UHBlueprintFunctionLibrary::GetSlowPercent(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->SlowPercent;
	}
	return false;
}

void UHBlueprintFunctionLibrary::SetSlowPercent(FGameplayEffectContextHandle& EffectContextHandle,
                                                  float bInIsSlowPercent)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SlowPercent = bInIsSlowPercent;
	}
}

bool UHBlueprintFunctionLibrary::GetIsKnockBack(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHGameplayEffectContext* HEffectContext = static_cast<const FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HEffectContext->bIsKnockBack;
	}
	return false;
}

void UHBlueprintFunctionLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetDebuffDamage(InDamage);
	}
}

void UHBlueprintFunctionLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetDebuffDuration(InDuration);
	}
}

void UHBlueprintFunctionLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UHBlueprintFunctionLibrary::SetDebuffTypeTag(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDebuffTypeTag)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DebuffType = MakeShared<FGameplayTag>(InDebuffTypeTag);
		HEffectContext->SetDebuffTypeTag(DebuffType);
	}
}

void UHBlueprintFunctionLibrary::SetDamageTypeTag(FGameplayEffectContextHandle& EffectContextHandle,
                                                  const FGameplayTag& InDamageTypeTag)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageTypeTag);
		HEffectContext->SetDamageTypeTag(DamageType);
	}
}

void UHBlueprintFunctionLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	if (FHGameplayEffectContext* HEffectContext = static_cast<FHGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HEffectContext->SetKnockbackForce(InForce);
	}
}

void UHBlueprintFunctionLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams,
	FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UHBlueprintFunctionLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		//DrawDebugSphere(World,SphereOrigin,Radius,12,FColor::Red,false,-1,0.f,1.f);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()) && Overlap.GetActor()->ActorHasTag(FName("Targetable")))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UHBlueprintFunctionLibrary::GetEnemyLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                                 TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                                 const FVector& SphereOrigin, AActor* OwnerActor)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		//DrawDebugSphere(World,SphereOrigin,Radius,12,FColor::Red,false,-1,0.f,1.f);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_IsDead(Overlap.GetActor()) &&
				Overlap.GetActor()->ActorHasTag(FName("Targetable")) &&
				IsEnemy(OwnerActor, Overlap.GetActor())
				)
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UHBlueprintFunctionLibrary::GetJungleLiveMobWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors,AActor* ActorToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(ActorToIgnore);
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		//DrawDebugSphere(World,SphereOrigin,Radius,12,FColor::Red,false,-1,0.f,1.f);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() &&
				!ICombatInterface::Execute_IsDead(Overlap.GetActor()) &&
				Overlap.GetActor()->ActorHasTag(FName("JungleMob")) 
				)
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

AActor* UHBlueprintFunctionLibrary::GetTowerWithinRadius(const UObject* WorldContextObject,
                                                         float Radius, const FVector& SphereOrigin)
{
	

	TArray<AActor*> OutOverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	
	GetLivePlayersWithinRadius(WorldContextObject,OutOverlappingActors,ActorsToIgnore,Radius,SphereOrigin);
	
	if (OutOverlappingActors.Num() == 0)
	{
		return nullptr;
	}
	
	for (auto Element : OutOverlappingActors)
	{
		if (Element->ActorHasTag(FName("Tower")))
		{
			return Element;
		}
	}
	
	return nullptr;
}

AActor* UHBlueprintFunctionLibrary::GetClosestTarget(AActor* Actor, const TArray<AActor*>& ActorsToIgnore,
                                                     float Radius)
{
	TArray<AActor*> OutOverlappingActors;
	GetLivePlayersWithinRadius(Actor,OutOverlappingActors,ActorsToIgnore,Radius,Actor->GetActorLocation());
	if (OutOverlappingActors.Num() == 0) return nullptr;

	if (OutOverlappingActors.Num() == 1 && IsEnemy(Actor,OutOverlappingActors[0]))
	{
		return OutOverlappingActors[0];
	}
	
	double ClosestDistance = TNumericLimits<double>::Max();
	AActor* ClosestActor = nullptr;
	
	for (auto& OutOverlappingActor : OutOverlappingActors)
	{
		if (IsEnemy(Actor,OutOverlappingActor))
		{
			float NewDistance = Actor->GetDistanceTo(OutOverlappingActor);
		
			if (NewDistance < ClosestDistance)
			{
				ClosestDistance = NewDistance;
				ClosestActor = OutOverlappingActor;
			}
		}
	}

	return ClosestActor;
	
}

AActor* UHBlueprintFunctionLibrary::Minion_GetClosestTarget(AActor* Actor, const TArray<AActor*>& ActorsToIgnore,
	float Radius)
{
	TArray<AActor*> OutOverlappingActors;
	GetEnemyLivePlayersWithinRadius(Actor,OutOverlappingActors,ActorsToIgnore,Radius,Actor->GetActorLocation(),Actor);
	
	if (OutOverlappingActors.Num() == 0)
	{
		return nullptr;
	}

	if (OutOverlappingActors.Num() == 1)
	{
		return OutOverlappingActors[0];
	}

	// Hedefteki aktörlerin hepsi düşman oyuncumu ??

	bool bIsAllenemyPlayer = true;

	for (auto& OutOverlappingActor : OutOverlappingActors)
	{
		if (!OutOverlappingActor->ActorHasTag(FName("Player")))
		{
			bIsAllenemyPlayer = false;
			break;
		}
	}

	double ClosestDistance = TNumericLimits<double>::Max();
	AActor* ClosestActor = nullptr;

	if (bIsAllenemyPlayer)
	{
		for (auto& OutOverlappingActor : OutOverlappingActors)
		{
			float NewDistance = Actor->GetDistanceTo(OutOverlappingActor);
		
			if (NewDistance < ClosestDistance)
			{
				ClosestDistance = NewDistance;
				ClosestActor = OutOverlappingActor;
			}
		}
	}

	// Hepsi enemy player değilse en yakındaki düşman minyon seçilecek
	
	for (auto& OutOverlappingActor : OutOverlappingActors)
	{
		if (IsEnemy(Actor,OutOverlappingActor) && !(OutOverlappingActor->ActorHasTag(FName("Player"))))
		{
			float NewDistance = Actor->GetDistanceTo(OutOverlappingActor);
		
			if (NewDistance < ClosestDistance)
			{
				ClosestDistance = NewDistance;
				ClosestActor = OutOverlappingActor;
			}
		}
	}
	
	return ClosestActor;
}

AActor* UHBlueprintFunctionLibrary::Tower_GetClosestTarget(AActor* Actor, const TArray<AActor*>& ActorsToIgnore,
	float Radius)
{
	TArray<AActor*> OutOverlappingActors;
	GetLivePlayersWithinRadius(Actor,OutOverlappingActors,ActorsToIgnore,Radius,Actor->GetActorLocation());
	
	if (OutOverlappingActors.Num() == 0)
	{
		return nullptr;
	}

	if (OutOverlappingActors.Num() == 1 && IsEnemy(Actor,OutOverlappingActors[0]))
	{
		return OutOverlappingActors[0];
	}
	
	double ClosestDistance = TNumericLimits<double>::Max();
	AActor* ClosestActor = nullptr;
	
	for (auto& OutOverlappingActor : OutOverlappingActors)
	{
		if (IsEnemy(Actor,OutOverlappingActor))
		{
			float NewDistance = Actor->GetDistanceTo(OutOverlappingActor);
		
			if (NewDistance < ClosestDistance)
			{
				ClosestDistance = NewDistance;
				ClosestActor = OutOverlappingActor;
			}
		}
	}
	
	return ClosestActor;
}

void UHBlueprintFunctionLibrary::H_AddReplicatedLooseTagToASC(UAbilitySystemComponent* ASC, FGameplayTag Tag)
{
	if (ASC)
	{
		ASC->AddReplicatedLooseGameplayTag(Tag);
	}
}

bool UHBlueprintFunctionLibrary::H_HaveASCAbilityWithThisTag(UAbilitySystemComponent* ASC, FGameplayTag Tag)
{
	check(ASC);

	for (auto& Element : ASC->GetActivatableAbilities())
	{
		if (Element.Ability->AbilityTags.HasTagExact(Tag)) return true;
	}

	return false;
}


FString UHBlueprintFunctionLibrary::GetFormatTimeAsText(const float TotalSeconds)
{
	int32 Hours = FMath::FloorToInt(TotalSeconds / 3600);
	int32 Minutes = FMath::FloorToInt(FMath::Fmod(TotalSeconds / 60, 60));
	int32 Seconds = FMath::FloorToInt(FMath::Fmod(TotalSeconds, 60));

	return FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
}

void UHBlueprintFunctionLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

void UHBlueprintFunctionLibrary::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets, AActor* OwnerActor, const FVector& TargetLoc)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);
	
	TArray<AActor*> OverlappingActors;
	
	GetEnemyLivePlayersWithinRadius(
	OwnerActor,
	OverlappingActors,
	ActorsToIgnore,
	850.f,
	TargetLoc,
	OwnerActor);
	
	int32 NumAdditionTargets = 6;
	
	GetClosestTargets(NumAdditionTargets, OverlappingActors, OutAdditionalTargets, TargetLoc);
}

bool UHBlueprintFunctionLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

bool UHBlueprintFunctionLibrary::IsEnemy(AActor* FirstActor, AActor* SecondActor)
{
	if (FirstActor == nullptr || SecondActor == nullptr)
	{
		return false;
	}

	if (FirstActor == SecondActor) return false;

	if (FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("JungleMob")))
	{
		return true;
	}

	if (FirstActor->ActorHasTag(FName("Minion")) && SecondActor->ActorHasTag(FName("JungleMob")))
	{
		return false;
	}
	
	if(FirstActor->Implements<UCombatInterface>() && SecondActor->Implements<UCombatInterface>())
	{
		bool bSameRadiant = ICombatInterface::Execute_IsRadiant(FirstActor) && ICombatInterface::Execute_IsRadiant(SecondActor);
		bool bSameDire = !ICombatInterface::Execute_IsRadiant(FirstActor) && !ICombatInterface::Execute_IsRadiant(SecondActor);

		return !(bSameRadiant || bSameDire);
	}
	
	return false;
}

void UHBlueprintFunctionLibrary::CancelAllActionAbilities(UAbilitySystemComponent* ASC)
{
	if (ASC->Implements<UHAbilitySystemInterface>())
	{
		IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
	}
}


TArray<FRotator> UHBlueprintFunctionLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UHBlueprintFunctionLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}