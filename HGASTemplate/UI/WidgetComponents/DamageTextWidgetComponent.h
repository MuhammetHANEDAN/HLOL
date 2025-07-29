// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetGoldText(int32 Gold);
	
};
