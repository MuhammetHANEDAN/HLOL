
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "HWidgetController.generated.h"

class AHBaseCharacter;
struct FGameplayTag;
class UAttributeSet;
class UAbilitySystemComponent;
class AHPlayerController;
class AHPlayerState;
class UHAbilitySystemComponent;
class UHAttributeSet;
class UAbilityInfo;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) : 
	PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerAttributeChangedSignature,  FGameplayTag, AttributeTag, int32, NewPoints);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedSignature, int32, NewStatValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFloatStatChangedSignature, float, NewStatValue);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class HGASTEMPLATE_API UHWidgetController : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

	void BroadcastAbilityInfo();

	UPROPERTY(BlueprintReadWrite, Category="WidgetController", meta=(ExposeOnSpawn="true"))
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AHPlayerController> HPlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AHPlayerState> HPlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UHAbilitySystemComponent> HAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UHAttributeSet> HAttributeSet;
	
	AHPlayerController* GetHPC();
	AHPlayerState* GetHPS();
	
	UFUNCTION(BlueprintPure)
	UHAbilitySystemComponent* GetHASC();
	
	UHAttributeSet* GetHAS();
	
	
};

