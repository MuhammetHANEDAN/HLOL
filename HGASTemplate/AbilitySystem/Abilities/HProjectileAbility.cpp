// Fill out your copyright notice in the Description page of Project Settings.


#include "HProjectileAbility.h"

#include "HGASTemplate/AbilitySystem/EffectActors/HProjectile.h"


void UHProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UHProjectileAbility::SpawnProjectile(const FVector& ProjectileSpawnLocation,
	const FRotator& ProjectileSpawnRotation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ProjectileSpawnLocation);
	SpawnTransform.SetRotation(ProjectileSpawnRotation.Quaternion());
		
	AHProjectile* Projectile = GetWorld()->SpawnActorDeferred<AHProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetAvatarActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->ActorLevel = GetAbilityLevel();
		
	Projectile->FinishSpawning(SpawnTransform);
}

