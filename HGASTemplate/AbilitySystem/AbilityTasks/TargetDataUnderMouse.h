
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

/**
 * Async Tasklere birden fazla output atanabiliyor ama atama işlemi için delegte gerekmektedir.
 */
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, Data);

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category="Ability|Task", meta=(DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility" , BlueprintInternalUseOnly = true))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData; // Yukardaki async taske out pin eklemiş olduk valid data diye
	// Ekstra olarak Data değerinide output olarak ekledi
	// Valid data broadcast olunca o Data değeride setlenip BP deki pine iletilmektedir.

private:

	virtual void Activate() override; // AbilityTask activate olduğunda çalışan fonk

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);










	
	
};

