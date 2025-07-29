
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "HGASTemplate/AbilitySystem/HAbilityTypes/HAbilityTypes.h"
#include "HEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotApply
};

/**
 * Infinity olarak sadece 1 adet etki uygulanabilir
 * Infintylerde dışardan spec vererek ge uygulamak istiyorsan onoverlapwithspec kullanmalısın ve orjinal infinity i boş bırakmalısın
 * 2 si beraber çalışmaz şu an
 */

UCLASS()
class HGASTEMPLATE_API AHEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AHEffectActor();

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AProps")
	bool bApplyEffectsToEnemies = false;

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	/** Just Infinities*/
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTargetWithSpec(AActor* TargetActor, const FGameplayEffectSpecHandle SpecHandle);

	/** Just Infinities*/
	UFUNCTION(BlueprintCallable)
	void OnOverlapWithSpec(AActor* TargetActor,const FGameplayEffectSpecHandle SpecHandle);

	/** Just Infinities*/
	UFUNCTION(BlueprintCallable)
	void OnEndOverlapWithSpec(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	bool bTargetIsEffectAble(AActor* TargetActor) const;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// Sadece Infinity effectlerde çalışacağı için 1 tane eklemiş olduk
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category="AProps",meta=(ExposeOnSpawn=true))
	int32 ActorLevel = 1.f;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true))
	FDamageEffectParams DamageEffectParams;

	


private:
	/**
	 * Eklenen infinite GE ler bir şekilde kaldırılmak istenirse bu actor kullanılarak
	 */
	UPROPERTY()
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandle;





	

};

