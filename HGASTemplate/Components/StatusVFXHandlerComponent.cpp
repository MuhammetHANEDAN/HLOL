// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusVFXHandlerComponent.h"

#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/Characters/HBaseCharacter.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"



UStatusVFXHandlerComponent::UStatusVFXHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UStatusVFXHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	/*if (GetOwner() == nullptr || bIsEffectAble == false) return;

	if (HASC)
	{
		BindTagDelegates();
	}
	else if (ICombatInterface* C = Cast<ICombatInterface>(GetOwner()))
	{
		C->GetOnASCRegisteredDelegate().AddWeakLambda(this,
			[this](UAbilitySystemComponent* ASC)
			{
				UHAbilitySystemComponent* L_hasc = Cast<UHAbilitySystemComponent>(ASC);
				if (L_hasc)
				{
					HASC = L_hasc;
					BindTagDelegates();
				}
			});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HASC not found STatusVFXHandle Comp"));;
	}
	*/
	
}

void UStatusVFXHandlerComponent::SetHASC(UAbilitySystemComponent* InHASC)
{
	HASC = Cast<UHAbilitySystemComponent>(InHASC);
}

void UStatusVFXHandlerComponent::BindTagDelegates()
{
	check(HASC);

	OwnerHBaseCharacter = Cast<AHBaseCharacter>(HASC->GetAvatarActor());
	
	const FHGamePlayTags& Tags = FHGamePlayTags::Get();

	HASC->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				OwnerHBaseCharacter->StunEnabled();
			}
			else
			{
				OwnerHBaseCharacter->StunDisabled();
			}
		});

	HASC->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Slow, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				OwnerHBaseCharacter->SLowEnabled();
			}
			else
			{
				OwnerHBaseCharacter->SLowDisabled();
			}
		});

	HASC->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Silence, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				
			}
			else
			{
				
			}
		});

	HASC->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Burn, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				OwnerHBaseCharacter->BurnEnabled();
			}
			else
			{
				OwnerHBaseCharacter->BurnDisabled();
			}
		});
	
}

void UStatusVFXHandlerComponent::InitStatusVFXHandler(UAbilitySystemComponent* ASC)
{
	HASC = Cast<UHAbilitySystemComponent>(ASC);
	BindTagDelegates();
}

