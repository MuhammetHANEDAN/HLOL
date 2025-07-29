// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HGASTemplate/AbilitySystem/HAbilityTypes/HAbilityTypes.h"
#include "HProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class HGASTEMPLATE_API AHProjectile : public AActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
public:	

	AHProjectile();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	/** Homing Projectile ise sadece işaretlenen hedefte etkileşime girecektir */
	bool bIsHomingProjectile = false;
	
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly, Category="Aprops")
	TWeakObjectPtr<AActor> HomingTargetActor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> HomingTargetAbilitySystemComponent = nullptr;
	
	virtual void Destroyed() override;


	/** General Check Functions */
	UFUNCTION(BlueprintCallable)
	bool OnOverlapSucced(AActor* OverlappedActor);
	UFUNCTION(BlueprintCallable)
	bool OnOverlapHomingSucced(AActor* OverlappedActor);

	UFUNCTION(BlueprintCallable)
	bool bTargetIsEffectAble(AActor* TargetActor) const;
	
	
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="Aprops")
	float LifeSpan = 6.f;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Aprops") // Belki ilerde kullanırız
	int32 ActorLevel = 1;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Aprops")
	float  NormalAttackSlowPercent = 0.f;

	











	
};
