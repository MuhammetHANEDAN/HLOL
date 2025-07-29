
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "HPlayerController.generated.h"




class UNavigationPath;
class UHAbilitySystemComponent;
class UHAttributeSet;
class UAttributeSet;
class UAbilitySystemComponent;
class ICombatInterface;
class AMobaCameraActor;
class UDamageTextWidgetComponent;
class UData_InputConfig;
class UNiagaraSystem;
class USplineComponent;
class UHInputConfig;
class UHInputComponent;
class IEnemyInterface;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API AHPlayerController : public APlayerController, public IPlayerInterface
{
	GENERATED_BODY()

public:

	AHPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void OnPossess(APawn* InPawn) override;
	

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:

	/** Player Interface*/
	virtual void AnimEvent_HandleNormalAttackAbility_Implementation() override;
	virtual void ContinueNormalAttack_Implementation() override;
	virtual void GameStarted_Implementation() override;
	virtual AActor* GetTargetActor_Implementation() override;
	virtual void I_ShowKillWidget_Implementation(const FGameplayTag& KillerTag, const FGameplayTag& VictimTag) override;
	virtual void I_ResetHPlayerControllerMovementAndTarget_Implementation() override;
	virtual void I_SetCameraLocationToPawn_Implementation() override;
	/** Player Interface*/

	UPROPERTY()
	APawn* ControlledPawn; // biz ekledik

	/**
	 * Ability System Works
	 */
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UHAbilitySystemComponent* GetHASC() const;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	UHAttributeSet* GetHAS() const;
	
	void AbilitySystemWorks();
	

	/**
	 * INPUT
	 */
	
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UData_InputConfig> InputConfig;

	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> HMappingContext;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> MouseScrollInputAction;

	void MouseScroll(const FInputActionValue& Value);

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	float MaxZoom = 1500.f;
	UPROPERTY(EditDefaultsOnly,Category="AProps")
	float MinZoom = 300.f;
	/** Camera ne kadar hızlı şekilde yakınlaşıp uzaklaşsın*/
	UPROPERTY(EditDefaultsOnly,Category="AProps")
	float ZoomSpeed= 5.f;

	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UInputAction> CancelAbilitiesInputAction;
	void Input_CancelAbilities(const FInputActionValue& Value);
	

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	void AutoRun();

	/**
	 * Target - Auto Run to Target
	 */
	
	bool bAutoRunningToTarget = false;
	//  ASC den alınacak
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float AttackRange = 50.f;
	void AutoRunToTarget();

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	UFUNCTION(Server,Reliable)
	void Server_SetTargetActor(AActor* TActor);

	UPROPERTY()
	TObjectPtr<UNavigationPath> TargetNavPath;
	
	/** Düşmana doğru hareket ederken NavPath düşmanın hareketine göre pathleri güncellemektedir.
	 * Her tickte NavPath tayin etmemek için periyot belirledik
	 * NAvpathi updatelememizin sebebi eğer update etmezsek target actorun yer değiştirmesine bağlı olarak spline componentten
	 * alınan direction bilgisi anlık olarak yanlış olup buga neden olabiliyor çözmek için böyle bir yol belirlendi
	 */
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float UpdateTargetNavPathThreshold = 0.1f;
	float UpdateTargetNavPathThresholdTrack = 0;
	bool bNeedUpdateTargetNavPath = false;

	void DisableAutoMovementToTarget();
	void EnableAutoMovementToTarget();
	
	bool bNormalAttackOnGoing = false;
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float TethetDistanceForNormalAttack = 50.f;
	
	


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	/**
	 * Cursor Trace
	 */
	void CursorTrace();
	FHitResult CursorHit;
	
	/*TScriptInterface<ICombatInterface> LastActor;
	TScriptInterface<ICombatInterface> ThisActor;*/

	UPROPERTY()
	TObjectPtr<AActor> LastActor = nullptr;
	UPROPERTY()
	TObjectPtr<AActor>  ThisActor = nullptr;

	/** Damage Indicator Works*/
	UFUNCTION(Client, Reliable)
	void Client_ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;

	/**
	 * Moba Camera
	 */

	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnRep_Pawn() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AMobaCameraActor> MobaCameraActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMobaCameraActor> MobaCameraActorClass;
	
	void MobaCameraMove(float DeltaTime);

	UFUNCTION(Client,Reliable)
	void Client_SetCameraLocationToCharacter(const FVector_NetQuantize& LocVector);

	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float ScrollCameraThreshold = 20.f;
	
	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	
	float CurrentMouseLocX = 0;
	float CurrentMouseLocY = 0;
	
	UPROPERTY(EditDefaultsOnly,Category="Properties")
	float MouseSpeed = 1500.f;
	
	// Global Camera Move Düzenleyici
	bool bCanMobaCameraMove = false;



	/** Moba Works*/
	
	/** Karakter Hangi Takımda*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Properties",Replicated)
	bool bIsRadiant = true;

	UPROPERTY()
	FGameplayTag HeroTag = FHGamePlayTags::Get().Heroes_Grux;
	
	UFUNCTION(Server,Reliable)
	void Server_SetIsRadiant(bool IsRadiant);

	UFUNCTION(Server,Reliable)
	void Server_SetHeroTag(FGameplayTag Tag);
	
	void InitializeControlledCharacterTeamAfterDelay();

	/**
	 * Delayed Works
	 */

	void DelayedSetviewTarget();

	UFUNCTION(Client,Reliable)
	void Client_DelayedSetviewTarget();

	//-----------------

	UFUNCTION(Client,Reliable)
	void Client_GameStarted();

	

	//-----------

	UFUNCTION(Client,Reliable)
	void Client_ShowKillWidget(const FGameplayTag& KillerTag, const FGameplayTag& VictimTag);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowKillWidget(const FGameplayTag& KillerTag, const FGameplayTag& VictimTag);

	UFUNCTION(BlueprintCallable)
	void ResetHPlayerControllerMovementAndTarget();
	
	void SetCameraLocationToPawn();

	UPROPERTY()
	TObjectPtr<UUserWidget> StartWidget = nullptr;

	UPROPERTY(EditDefaultsOnly,Category="AProps")
	TSubclassOf<UUserWidget> StartWidgetClass;
	
	
};

