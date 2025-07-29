// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Data_InputConfig.generated.h"

struct FGameplayTag;
class UInputAction;

USTRUCT(BlueprintType)
struct FData_InputConfigInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
	
};

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UData_InputConfig : public UDataAsset
{
	GENERATED_BODY()
	

public:

	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FData_InputConfigInputAction> AbilityInputActions;


	

	
	
};
