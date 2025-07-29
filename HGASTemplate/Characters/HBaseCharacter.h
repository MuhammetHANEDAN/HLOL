// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ScalableFloat.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HBaseCharacter.generated.h"

class UStatusVFXHandlerComponent;
class UDamageTextWidgetComponent;
class UHAttributeSet;
struct FScalableFloat;
class UBarComponent;
struct FGameplayTag;
class FOnDeathSignature;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE (FOnLifeSignature);

UCLASS()
class HGASTEMPLATE_API AHBaseCharacter : public ACharacter, public ICombatInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

	public:
	
	AHBaseCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

protected:
	
	virtual void BeginPlay() override;

public:

	virtual void PostInitializeComponents() override;
	
	/** Combat Interface */
	/*virtual void HighlightActor_Implementation(bool bEnemy) override;
	virtual void UnHighlightActor_Implementation() override;*/
	virtual bool IsRadiant_Implementation() override;
	virtual FOnFloatValueSignature& GetLevelChangedDelegate() override;
	virtual void H_HighlightActor_Implementation(bool bEnemy) override;
	virtual void H_UnHighlightActor_Implementation() override;
	/** Server_SetTeam RPC yi çağırır*/
	virtual void SetIsRadiant_Implementation(bool bRadiant) override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual void Die(const FVector& DeathImpulse, AActor* KillerActor) override;
	virtual void ReSpawn(const FVector& Location, const FRotator& Rotation) override;
	virtual bool IsDead_Implementation() const override;
	virtual UHAttributeSet* I_GetHAS_Implementation() override;
	virtual FVector GetCombatSocketLocation_Implementation(FName SocketName) override;
	virtual FGameplayTag GetHeroTag_Implementation() const override;

	
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	/** Combat Interface */

	/** Ability System Interface*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** Ability System Interface*/
	
	FOnASCRegistered OnAscRegistered;
	
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
	/**
	 * Player Characterlerin Level kısmı PS de bulunuyor Ama PS Simulated Proxylerde bulunmuyor
	 * Bilginin Sİm Proxylere aktarılması için kullanılıyor
	 */
	FOnFloatValueSignature OnLevelChanged;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AProps")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AProps")
	TObjectPtr<UAttributeSet> AttributeSet;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	UHAttributeSet* GetHAS() const;
	
	virtual void InitAbilityActorInfo();

	/** ASC ve AS initialize edildikten sonra çağırılacak olan fonksiyon */
	virtual void AbilitySystemInitialized(const UAbilitySystemComponent* ASC);

	/** Controller Classın içindeki AbilitySystem Workleri başlatmak için*/
	virtual void InitializeControllerAS();
	
	void ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> GamePlayEffectClass, float Level) const;

	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();
	
	/** Statların initialize edilme işlemi GE ler kullanılarak yapılıyor Bağımsızlar vital gibi olanlar instant effect
	 * bağımlı olanlar infinite effekt olarak uyguladık
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	TSubclassOf<UGameplayEffect> DefaultAttributes;

	/**
	 * Health ve Mana MaxHealth ve MaxManaya Bağlı 
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/** Karaktere eklenecek olan abilitylerin class arrayi, Karakterin passivesi de buna dahildir */
	UPROPERTY(EditAnywhere,Category="AProps")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	/** Run time boyu çalışacak passive abilityler Event dinleyici ability olarak kullandık */
	UPROPERTY(EditAnywhere, Category = "AProps")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	/**
	 * DEATH WORKS
	 */

	UPROPERTY(Replicated)
	bool bDead = false;
	
	UFUNCTION(NetMulticast,Reliable)
	virtual void Multicast_HandleDeath(const FVector& DeathImpulse);
	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TObjectPtr<UAnimMontage> DeathMontage;

	


	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AProps")
	float CurrentWalkSpeed = 600.f;
	

	/** Moba Works*/
	
	/** Karakter Hangi Takımda*/
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="AProps")
	bool bIsRadiant = true;
	UFUNCTION(Server,Reliable)
	void Server_SetTeam(bool bRadiant);

	/** XP - Level*/

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AProps")
	FScalableFloat XpLevelCurveTable;



	// ---------------
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(Client,Reliable)
	void Client_ShowGoldNumber(int32 GoldAmount, ACharacter* TargetCharacter);
	
	UPROPERTY(EditDefaultsOnly, Category="AProps")
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DamageIndicatorShowComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void DebugEventOnnOff(bool bOn);


	/**
	 * StatusVFXHandler
	 */
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AProps")
	TObjectPtr<UStatusVFXHandlerComponent> StatusVFXHandlerComponent;

	UFUNCTION(BlueprintImplementableEvent)
	void StunEnabled();
	UFUNCTION(BlueprintImplementableEvent)
	void StunDisabled();

	UFUNCTION(BlueprintImplementableEvent)
	void SLowEnabled();
	UFUNCTION(BlueprintImplementableEvent)
	void SLowDisabled();

	UFUNCTION(BlueprintImplementableEvent)
	void BurnEnabled();
	UFUNCTION(BlueprintImplementableEvent)
	void BurnDisabled();

	/** Kuleler için devre dışı bırakılacak*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AProps")
	bool bIsEffectAble = true;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UFUNCTION(BlueprintImplementableEvent)
	void CharacterKilledBy(AActor* KillerActor);

	UFUNCTION(BlueprintImplementableEvent)
	void Multicast_TowerExplosion();

	//
	/*FOnLifeSignature OnAliveDelegate;
	FOnLifeSignature OnDeadDelegate;*/  //?
	
	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TSubclassOf<UGameplayEffect> DeathEffect;

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	FGameplayTag HeroTag = FHGamePlayTags::Get().Heroes_Grux;

	

};
