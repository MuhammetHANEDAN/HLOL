// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "HAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


//typedef TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template <class T>
using TStaticFuncPtr = TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy>::FFuncPtr;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	
	UPROPERTY()
	AController* SourceController = nullptr;
	
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	
	UPROPERTY()
	AController* TargetController = nullptr;
	
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	public:

	UHAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Pre - Değişim yapılmadan önce firelanıyor yani burada değişecek olan değere müdahale edilebilir
	 * Epic bu fonksiyonu clamp işleri için öneriyor
	 */
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	//TMap<FGameplayTag,TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	// FGameplayAttribute(*)() = TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy>::FFuncPtr kısaltmış olduk
	//TMap<FGameplayTag,FGameplayAttribute(*)()> TagsToAttributes;
	// typedef kullnarak dahada okunaklı hale soktuk -> FAttributeFuncPtr typedef en yukarda.
	// sonra usingle templateye dönüştürdük
	// TStaticFuncPtr<FGameplayAttribute()> bu işaret edilecek fonk FGameplayAttribute döndürüyor input almıyor
	// input alsaydı () içine eklemeliydin (float, int32, AActor*) gibi isim vermeden
	TMap<FGameplayTag,TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	

	/**
	 * Fonksiyon pointer elde edebilirsin böylece
	*FunctionPointer = GetStrengthAttribute; FunctionPointer sağdaki fonku işaret ediyor
	FunctionPointer(); Çağırlınca GetStrengthAttribute bu fonkun döndürdüğü değeri döndürecek
	 *
	 */
	//TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy>::FFuncPtr FunctionPointer;

	/**
	 * Attributes 
	 */

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Primary Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Primary Attributes")
	FGameplayAttributeData HealthRegen;
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, HealthRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Primary Attributes")
	FGameplayAttributeData MaxMana;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, MaxMana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen, Category = "Primary Attributes")
	FGameplayAttributeData ManaRegen;
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, ManaRegen);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackDamage, Category = "Primary Attributes")
	FGameplayAttributeData AttackDamage;
	UFUNCTION()
	void OnRep_AttackDamage(const FGameplayAttributeData& OldAttackDamage) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, AttackDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackSpeed, Category = "Primary Attributes")
	FGameplayAttributeData AttackSpeed;
	UFUNCTION()
	void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, AttackSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AbilityPower, Category = "Primary Attributes")
	FGameplayAttributeData AbilityPower;
	UFUNCTION()
	void OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, AbilityPower);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackRange, Category = "Primary Attributes")
	FGameplayAttributeData AttackRange;
	UFUNCTION()
	void OnRep_AttackRange(const FGameplayAttributeData& OldAttackRange) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, AttackRange);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalArmor, Category = "Primary Attributes")
	FGameplayAttributeData PhysicalArmor;
	UFUNCTION()
	void OnRep_PhysicalArmor(const FGameplayAttributeData& OldPhysicalArmor) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, PhysicalArmor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalArmor, Category = "Primary Attributes")
	FGameplayAttributeData MagicalArmor;
	UFUNCTION()
	void OnRep_MagicalArmor(const FGameplayAttributeData& OldMagicalArmor) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, MagicalArmor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category = "Primary Attributes")
	FGameplayAttributeData CriticalHitChance;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamageCoefficient, Category = "Primary Attributes")
	FGameplayAttributeData CriticalHitDamageCoefficient;
	UFUNCTION()
	void OnRep_CriticalHitDamageCoefficient(const FGameplayAttributeData& OldCriticalHitDamageCoefficient) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, CriticalHitDamageCoefficient);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LifeSteal, Category = "Primary Attributes")
	FGameplayAttributeData LifeSteal;
	UFUNCTION()
	void OnRep_LifeSteal(const FGameplayAttributeData& OldLifeSteal) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, LifeSteal);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CooldownReduction, Category = "Primary Attributes")
	FGameplayAttributeData CooldownReduction;
	UFUNCTION()
	void OnRep_CooldownReduction(const FGameplayAttributeData& OldCooldownReduction) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, CooldownReduction);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_PhysicalArmorPenetration, Category = "Primary Attributes")
	FGameplayAttributeData PhysicalArmorPenetration;
	UFUNCTION()
	void OnRep_PhysicalArmorPenetration(const FGameplayAttributeData& OldPhysicalArmorPenetration) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, PhysicalArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicalArmorPenetration, Category = "Primary Attributes")
	FGameplayAttributeData MagicalArmorPenetration;
	UFUNCTION()
	void OnRep_MagicalArmorPenetration(const FGameplayAttributeData& OldMagicalArmorPenetration) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, MagicalArmorPenetration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, Category = "Primary Attributes")
	FGameplayAttributeData MoveSpeed;
	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, MoveSpeed);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BashChance, Category = "Primary Attributes")
	FGameplayAttributeData BashChance;
	UFUNCTION()
	void OnRep_BashChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, BashChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BashChance, Category = "Primary Attributes")
	FGameplayAttributeData NormalAttackSlow;
	UFUNCTION()
	void OnRep_NormalAttackSlow(const FGameplayAttributeData& OldNormalAttackSlow) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, NormalAttackSlow);


	/**
	 * Vital Attributes
	 */
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health , Category="Vital Attributes")
	FGameplayAttributeData Health;
	UFUNCTION( )
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet,Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Mana , Category="Vital Attributes")
	FGameplayAttributeData Mana;
	UFUNCTION( )
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	ATTRIBUTE_ACCESSORS(UHAttributeSet,Mana);
	

	/**
	 * Meta Attributes
	 * meta attributes are not replicated. We set them on the server and then
	 * on the server we process the data and then change any replicated attributes.Based on  calculations.
	 */

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, IncomingDamage);

	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, IncomingXP);

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingHeal;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, IncomingHeal);

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingLifeSteal;
	ATTRIBUTE_ACCESSORS(UHAttributeSet, IncomingLifeSteal);

private:

	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void HandleIncomingHeal(const FEffectProperties& Props);
	void HandleIncomingLifeSteal(const FEffectProperties& Props);

	
	/** GAS ile ilgili güzel bir fonksiyon oldu göz gezdir özellikle GE lerin C++ tan oluşturulması ve ayarlanması*/
	void Debuff(const FEffectProperties& Props);

	void DebuffControl(const FEffectProperties& Props);
	
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);

	/** Damage Indicator Works*/
	void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void ShowFloatingTextHeal(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;

	bool ShouldShowDamageIndicator(const FEffectProperties& Props) const;

	void SendXPEvent(const FEffectProperties& Props);

	bool bTopOffHealth = false;
	bool bTopOffMana = false;

	void AddGoldToKiller (const FEffectProperties& Props);

	void CheckForJungleMob (const FEffectProperties& Props);
	void CheckForTower(const FEffectProperties& Props);
	
};
