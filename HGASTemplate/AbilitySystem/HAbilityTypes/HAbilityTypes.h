#pragma once
#include "AbilitySystemComponent.h"
#include "HAbilityTypes.generated.h"

class UGameplayEffect;

/**
 *
 */
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDebuff = false;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageTypeTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DebuffTypeTag = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsKnockBack = false;

	UPROPERTY(BlueprintReadWrite)
	bool bIsSlow = false;

	UPROPERTY(BlueprintReadWrite)
	float SlowPercent = 0.f;
	
};

/**
*Oluşturduğumz custom EffectContext structımız
*ASC -> MakeEffectContext fonksiyonu çalıştırılınca bu türden bir EffectContext Döndürecek
*Bu classa GAS taki fonk ve classlar arası iletişimde iletmek istediğimiz custom özellikleri iletmemize yarayacak
*bCriticalHti , bDebuffed vs ...
*Etkinleştirmek için AbilitySystemGlobalstan derived Child sınıf alduk AuraAbilitySystemGlobals Diye
*Sonra onu GameDefaults.ini De etkinleştirdik
*Riderdan h ve cpp alınarak oluşturuldu Editörden başlatılmadı
*/
USTRUCT(BlueprintType)
struct FHGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit () const { return bIsBlockedHit; }
	bool IsDebuff() const { return bIsDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDebuffTypeTag() const { return DebuffTypeTag; }
	TSharedPtr<FGameplayTag> GetDamageTypeTag() const { return DamageTypeTag; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool GetIsBashHit() const { return bIsBashHit; }
	

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsDebuff(bool bInIsDebuff) { bIsDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDebuffTypeTag(TSharedPtr<FGameplayTag> InDebuffTypeTag) { DebuffTypeTag = InDebuffTypeTag; }
	void SetDamageTypeTag(TSharedPtr<FGameplayTag> InDamageTypeTag) { DamageTypeTag = InDamageTypeTag; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }
	void SetIsBashHit(bool bInIsBashHit) { bIsBashHit = bInIsBashHit; }
	
	
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
		//return FHGameplayEffectContext::StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const
	{
		FHGameplayEffectContext* NewContext = new FHGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
public:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageTypeTag;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bIsBashHit = false;

	TSharedPtr<FGameplayTag> DebuffTypeTag;

	UPROPERTY()
	bool bIsSlow = false;

	UPROPERTY()
	float SlowPercent = 0.f;

	UPROPERTY()
	bool bIsKnockBack = false;
	
	
};


// Important
template<>
struct TStructOpsTypeTraits<FHGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FHGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
