// Fill out your copyright notice in the Description page of Project Settings.


#include "HBaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HGASTemplate/HGASTemplate.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Components/StatusVFXHandlerComponent.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/UI/WidgetComponents/BarComponent.h"
#include "HGASTemplate/UI/WidgetComponents/DamageTextWidgetComponent.h"
#include "HGASTemplate/Components/StatusVFXHandlerComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"


AHBaseCharacter::AHBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	const FHGamePlayTags& GameplayTags = FHGamePlayTags::Get();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	DamageIndicatorShowComponent = CreateDefaultSubobject<USceneComponent>("DamageIndicatorShowComponent");
	DamageIndicatorShowComponent->SetupAttachment(GetRootComponent());
	
	StatusVFXHandlerComponent = CreateDefaultSubobject<UStatusVFXHandlerComponent>("StatusVFX Handler Component");

	Tags.Add(FName("Targetable"));
	
}

void AHBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHBaseCharacter,bIsRadiant);
	DOREPLIFETIME(AHBaseCharacter,bDead);
}

void AHBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AHBaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

bool AHBaseCharacter::IsRadiant_Implementation()
{
	return bIsRadiant;
}

FOnFloatValueSignature& AHBaseCharacter::GetLevelChangedDelegate()
{
	return OnLevelChanged;
}

void AHBaseCharacter::H_HighlightActor_Implementation(bool bEnemy)
{
	GetMesh()->SetRenderCustomDepth(true);
	if(bEnemy)
	{
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
	else
	{
		GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
	}
}

void AHBaseCharacter::H_UnHighlightActor_Implementation()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void AHBaseCharacter::SetIsRadiant_Implementation(bool bRadiant)
{
	Server_SetTeam(bRadiant);
}

float AHBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

void AHBaseCharacter::Server_SetTeam_Implementation(bool bRadiant)
{
	bIsRadiant = bRadiant;
}

UAbilitySystemComponent* AHBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UHAttributeSet* AHBaseCharacter::GetHAS() const
{
	if(AttributeSet )
	{
		return Cast<UHAttributeSet>(AttributeSet);
	}
	return nullptr;
}

void AHBaseCharacter::InitAbilityActorInfo()
{
	
}

void AHBaseCharacter::AbilitySystemInitialized(const UAbilitySystemComponent* ASC)
{
	
}

void AHBaseCharacter::InitializeControllerAS()
{
	
}

void AHBaseCharacter::ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> GamePlayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	if(GamePlayEffectClass == nullptr) return;
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this); // important
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GamePlayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AHBaseCharacter::InitializeDefaultAttributes() const
{
	ApplyGamePlayEffectToSelf(DefaultAttributes,1);
	ApplyGamePlayEffectToSelf(DefaultVitalAttributes,1);
}

void AHBaseCharacter::AddCharacterAbilities()
{
	UHAbilitySystemComponent* HASC = CastChecked<UHAbilitySystemComponent>(AbilitySystemComponent);
	if(!HasAuthority()) return;

	if(StartupAbilities.Num() == 0) return;

	HASC->AddCharacterAbilities(StartupAbilities);
	HASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

FOnDeathSignature& AHBaseCharacter::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

void AHBaseCharacter::Die(const FVector& DeathImpulse, AActor* KillerActor)
{
	if (HasAuthority() == false) return;
	
	if (AbilitySystemComponent)
	{
		IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
		//AbilitySystemComponent->AddReplicatedLooseGameplayTag(FHGamePlayTags::Get().State_Dead); ?
		ApplyGamePlayEffectToSelf(DeathEffect,1);
	}
	
	bDead = true;
	Multicast_HandleDeath(DeathImpulse);

	if (KillerActor && KillerActor->ActorHasTag(FName("Player")) &&  ActorHasTag(FName("JungleMob")))
	{
		CharacterKilledBy(KillerActor);
	}
	
}

void AHBaseCharacter::ReSpawn(const FVector& Location, const FRotator& Rotation) // Sadece Player Characterler için geçerli olacak
{
	
}

void AHBaseCharacter::Multicast_HandleDeath_Implementation(const FVector& DeathImpulse)
{

	//OnDeadDelegate.Broadcast(); ? 
	
	//GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Projectile
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore); // Minion
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); // Skill

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);

	if(DeathMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(DeathMontage);
	}
	
	OnDeathDelegate.Broadcast(this);

	GetCharacterMovement()->SetAvoidanceEnabled(false);
}

FOnASCRegistered& AHBaseCharacter::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

bool AHBaseCharacter::IsDead_Implementation() const
{
	return bDead;
}

UHAttributeSet* AHBaseCharacter::I_GetHAS_Implementation()
{
	return nullptr;
}

FVector AHBaseCharacter::GetCombatSocketLocation_Implementation(FName SocketName)
{
	return GetMesh()->GetSocketLocation(SocketName);
}

FGameplayTag AHBaseCharacter::GetHeroTag_Implementation() const
{
	return HeroTag;
}

AActor* AHBaseCharacter::GetAvatar_Implementation()
{
	return this;
}


FOnDamageSignature& AHBaseCharacter::GetOnDamageSignature()
{
	return OnDamageDelegate;
}

void AHBaseCharacter::Multicast_ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
	bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter))
	{
		if (AHBaseCharacter* TC = Cast<AHBaseCharacter>(TargetCharacter))
		{
			if (TC->DamageTextComponentClass == nullptr) return;
			UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TC->DamageIndicatorShowComponent, FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageText->SetDamageText(DamageAmount,bBlockedHit,bCriticalHit);
		}
	}
}

void AHBaseCharacter::Client_ShowGoldNumber_Implementation(int32 GoldAmount, ACharacter* TargetCharacter)
{
	if (IsValid(TargetCharacter))
	{
		if (AHBaseCharacter* TC = Cast<AHBaseCharacter>(TargetCharacter))
		{
			if (TC->DamageTextComponentClass == nullptr) return;
			UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
			DamageText->RegisterComponent();
			DamageText->AttachToComponent(TC->DamageIndicatorShowComponent, FAttachmentTransformRules::KeepRelativeTransform);
			DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			DamageText->SetGoldText(GoldAmount);
		}
	}
}

void AHBaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	if (HasAuthority() == false) return;

	if (bIsEffectAble == false && !IsValid(AbilitySystemComponent) && !AbilitySystemComponent->Implements<UHAbilitySystemInterface>()) return;

	FHGamePlayTags FhGamePlayTags = FHGamePlayTags::Get();
	
	if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	{
		IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
		
		AbilitySystemComponent->AddReplicatedLooseGameplayTag(FhGamePlayTags.Debuff_Knockback);
	}
	
	else
	{
	
		/*if (AbilitySystemComponent->HasAnyMatchingGameplayTags(FhGamePlayTags.Debuff_Knockback.GetSingleTagContainer()))
		{
			HLOG("Falling Cleared");
			AbilitySystemComponent->RemoveReplicatedLooseGameplayTag(FhGamePlayTags.Debuff_Knockback);
		}*/

		AbilitySystemComponent->RemoveReplicatedLooseGameplayTag(FhGamePlayTags.Debuff_Knockback);
	}
	
}









