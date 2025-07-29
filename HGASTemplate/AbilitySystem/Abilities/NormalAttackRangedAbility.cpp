// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAttackRangedAbility.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "HGASTemplate/AbilitySystem/EffectActors/HProjectile.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"

void UNormalAttackRangedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UNormalAttackRangedAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride)
{
	/*const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo());
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AHProjectile* Projectile = GetWorld()->SpawnActorDeferred<AHProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
	Projectile->FinishSpawning(SpawnTransform);*/
}

void UNormalAttackRangedAbility::SpawnProjectileHoming(AActor* HomingTargetActor, const FVector& SocketLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	if (HomingTargetActor == nullptr) return;

	FRotator Rotation = (HomingTargetActor->GetActorLocation() - SocketLocation).Rotation();
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AHProjectile* Projectile = GetWorld()->SpawnActorDeferred<AHProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	Projectile->ActorLevel = GetAbilityLevel();
	Projectile->NormalAttackSlowPercent = GetAttributeValueByTag(FHGamePlayTags::Get().Attributes_NormalAttackSlow);

	Projectile->DamageEffectParams.BaseDamage = GetAttributeValueByTag(FHGamePlayTags::Get().Attributes_AttackDamage);

	Projectile->ProjectileMovement->HomingTargetComponent = HomingTargetActor->GetRootComponent();
	Projectile->ProjectileMovement->HomingAccelerationMagnitude = HomingAcceleration;
	Projectile->ProjectileMovement->bIsHomingProjectile = true;

	Projectile->bIsHomingProjectile = true;
	Projectile->HomingTargetActor = HomingTargetActor;
		
	Projectile->FinishSpawning(SpawnTransform);
}
