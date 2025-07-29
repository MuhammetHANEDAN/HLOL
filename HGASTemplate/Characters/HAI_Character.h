
#pragma once

#include "CoreMinimal.h"
#include "HBaseCharacter.h"
#include "HGASTemplate/Interfaces/AI_Interface.h"
#include "HAI_Character.generated.h"


class AHJungleSpawner;
class AHSpawner;
class UMinionBarComponent;
class UBehaviorTreeComponent;
class AHAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API AHAI_Character : public AHBaseCharacter, public IAI_Interface
{
	GENERATED_BODY()

public:

	AHAI_Character();

protected:

	virtual void BeginPlay() override;

public:
	
	virtual void PossessedBy(AController* NewController) override;
	

	/** ai interface*/
	
	/** ai interface*/

	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse, AActor* KillerActor) override;
	virtual float GetXPReward_Implementation() override;
	virtual UHAttributeSet* I_GetHAS_Implementation() override;
	virtual int32 GetGoldReward_Implementation() override;
	virtual void SetTargetActor_Implementation(AActor* TActor) override;
	virtual AActor* GetTargetCharacter_Implementation() override;
	virtual void H_HighlightActor_Implementation(bool bEnemy) override;
	virtual void H_UnHighlightActor_Implementation() override;
	/** Combat Interface */

	/** IAbility Interface*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** IAbility Interface*/

	/**
	 * Ability System Works
	 */

	virtual void AbilitySystemInitialized(const UAbilitySystemComponent* ASC) override;

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	
	
	/**
	 * Replicated yapmadık çünkü zaten sadece serverde hesaplamalar yapılacak Ve runtimeda değişiklik olmayacak
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	int32 Level = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	bool bIsMinion = true;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	bool bIsMelee = true;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	bool bIsDragon = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	bool bIsTower = false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="AProps")
	bool bIsJungleMob = false;

	/**
	 * AI works
	 */

	UPROPERTY(EditAnywhere,Category="Aprops")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AHAIController> HAIController;

	/** Worlddeki Middle Spline ı begin playde almak için */
	UFUNCTION(BlueprintImplementableEvent)
	void BBInitialized();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite)
	TWeakObjectPtr<AActor> TargetActor;

	UPROPERTY(EditDefaultsOnly,Category="AProps|AI")
	FVector EndTargetLocation = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly,Category="AProps|AI")
	float ChaseDistance = 1000.f;

	UPROPERTY(EditDefaultsOnly,Category="AProps|AI")
	float AttackTetherDistance = 30.f;

	UPROPERTY(EditDefaultsOnly,Category="AProps|AI")
	float DetectEnemyRange = 800.f;

	UPROPERTY(EditDefaultsOnly,Category="AProps|AI")
	float TowerAttackRange = 1200.f;
	

	/**
	 * Minion Bar Comp
	 */

	UPROPERTY(VisibleAnywhere,Category="AProps")
	TObjectPtr<UMinionBarComponent> MinionBarComponent;

	UPROPERTY()
	TWeakObjectPtr<AHJungleSpawner> Spawner = nullptr;

	//----------------

	UFUNCTION(BlueprintImplementableEvent)
	void H_SetCustomDepthStencilValue(float Value);
	UFUNCTION(BlueprintImplementableEvent)
	void H_SetRenderCustomDepth(bool bOpen);
	
	
	
};
