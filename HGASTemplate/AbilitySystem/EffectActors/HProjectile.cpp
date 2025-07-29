
#include "HProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/AudioComponent.h"
#include "HGASTemplate/AbilitySystem/HAbilityTypes/HAbilityTypes.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"


AHProjectile::AHProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
}

void AHProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() && bIsHomingProjectile)
	{
		if (!HomingTargetActor.IsValid() || !HomingTargetAbilitySystemComponent.IsValid() ||
			ICombatInterface::Execute_IsDead(HomingTargetActor.Get()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Projectile yıkılacak"));
			Destroy();
		}
	}
}

void AHProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{
		SetLifeSpan(LifeSpan);

		if (bIsHomingProjectile && HomingTargetActor.IsValid())
		{
			HomingTargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HomingTargetActor.Get());
		}
	}
	
	SetReplicateMovement(true);
}



void AHProjectile::Destroyed()
{
	Super::Destroyed();
}

bool AHProjectile::OnOverlapSucced(AActor* OverlappedActor)
{

	if (UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlappedActor) == nullptr) return false;
	
	//if (OverlappedActor == Owner) return false  ? Belki kendinede overlap olması gerekiyor

	// BP de olası checkler
	// OverlappedActor == Owner , Enemy ?

	return true; // Destroy BP den gerçekleşecek
}

bool AHProjectile::OnOverlapHomingSucced(AActor* OverlappedActor)
{
	if (OverlappedActor == Owner || bIsHomingProjectile == false) return false;
		
	if (OverlappedActor == HomingTargetActor) return true;
	
	return false; // Destroy BP den gerçekleşecek
}

bool AHProjectile::bTargetIsEffectAble(AActor* TargetActor) const
{
	if (TargetActor->ActorHasTag(FName("Tower")) ) return false;
	return true;
}

