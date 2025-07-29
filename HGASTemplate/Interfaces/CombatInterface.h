// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UHAttributeSet;
struct FHeroDataStruct;
class UAbilitySystemComponent;
class UNiagaraSystem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatValueSignature, float /*Value*/);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType) // BP Type koyduk bp de cast edebilelim diye
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HGASTEMPLATE_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** 2 takım oldugu için Radiant ve Dire takım bilgisini bool ile çekiyoruz*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsRadiant();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetIsRadiant(bool bRadiant);

	/** Highlight edilmek istenen actor higlight eden actor ile aynı takımda ise true bilgisi yolla*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HighlightActor(bool bSameTeam );

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UnHighlightActor();

	/** Highlight edilmek istenen actor higlight eden actor ile aynı takımda ise true bilgisi yolla*/
	UFUNCTION(BlueprintNativeEvent)
	void H_HighlightActor(bool bEnemy );

	UFUNCTION(BlueprintNativeEvent)
	void H_UnHighlightActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetTargetCharacter();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTargetActor(AActor* TActor);
	
	virtual FOnFloatValueSignature& GetLevelChangedDelegate() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTag GetHeroTag() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // bp imp event yapınca virtual keywordu koymuyorsun
	void UpdateFacingTarget(const AActor* Target);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) 
	void UpdateFacingTargetLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) // Actor öldüğünde ne kadarlık exp vereceği
	float GetXPReward();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetGoldReward();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;

	virtual void Die(const FVector& DeathImpulse, AActor* KillerActor) = 0;
	virtual void ReSpawn(const FVector& Location, const FRotator& Rotation) = 0;
	
	UFUNCTION(BlueprintNativeEvent)
	UHAttributeSet* I_GetHAS();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FVector GetCombatSocketLocation(FName SocketName);


	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetPlayerLevel();
	
	

	/**
	 * Now, Blueprint Native event means that we don't have to mark this as virtual blueprint.
	 * Native event is going to automatically generate a virtual native version that exists in C plus plus
	 * that we can override here in C plus plus.
	 * And that's the version that has underscore implementation at the end of it.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	virtual FOnDamageSignature& GetOnDamageSignature() = 0; 


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	int32 GetMinionCount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void IncremenetMinionCount(int32 Amount);

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeapon();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShocked() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShocked(bool bInShock);
	
};
