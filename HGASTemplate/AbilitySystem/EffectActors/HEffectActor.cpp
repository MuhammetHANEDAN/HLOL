
#include "HEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


AHEffectActor::AHEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	bReplicates = true;

}

void AHEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AHEffectActor::Destroyed()
{
	if (ActiveEffectHandle.Num() > 0)
	{
		for (auto HandlePair : ActiveEffectHandle)
		{
			if (IsValid(HandlePair.Value))
			{
				HandlePair.Value->RemoveActiveGameplayEffect(HandlePair.Key, 1);
			}
		}
	}
}

void AHEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (GetOwner() == nullptr) return;
	
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	
	if(TargetASC == nullptr) return;
	check(GameplayEffectClass);
	
	FGameplayEffectContextHandle EffectContextHandle = OwnerASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(OwnerASC->GetAvatarActor()); // Bu efekti hangi actor yaptı
	
	const FGameplayEffectSpecHandle EffectSpecHandle = OwnerASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel,EffectContextHandle);
	
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	if(bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandle.Add(ActiveGameplayEffectHandle, TargetASC);
	}
	
}

void AHEffectActor::OnOverlap(AActor* TargetActor)
{
	check(TargetActor);
	
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
}

void AHEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if(DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	if(InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargetASC)) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;

		for (auto HandlePair : ActiveEffectHandle)
		{
			if(TargetASC == HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for ( FActiveGameplayEffectHandle& Handle : HandlesToRemove)
		{
			ActiveEffectHandle.FindAndRemoveChecked(Handle);
		}
	}
}

void AHEffectActor::ApplyEffectToTargetWithSpec(AActor* TargetActor, const FGameplayEffectSpecHandle SpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC == nullptr) return;

	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	const bool bIsInfinite = SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;
	
	if(bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandle.Add(ActiveGameplayEffectHandle, TargetASC);
	}
	
}

void AHEffectActor::OnOverlapWithSpec(AActor* TargetActor, const FGameplayEffectSpecHandle SpecHandle)
{
	ApplyEffectToTargetWithSpec(TargetActor,SpecHandle);
}

void AHEffectActor::OnEndOverlapWithSpec(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!IsValid(TargetASC)) return;

	TArray<FActiveGameplayEffectHandle> HandlesToRemove;

	for (auto HandlePair : ActiveEffectHandle)
	{
		if(TargetASC == HandlePair.Value)
		{
			TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
			HandlesToRemove.Add(HandlePair.Key);
		}
	}

	for ( FActiveGameplayEffectHandle& Handle : HandlesToRemove)
	{
		ActiveEffectHandle.FindAndRemoveChecked(Handle);
	}
}

bool AHEffectActor::bTargetIsEffectAble(AActor* TargetActor) const
{
	if (TargetActor->ActorHasTag(FName("Tower")) || TargetActor->ActorHasTag(FName("Dragon"))) return false;
	return true;
}
