
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AI_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAI_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HGASTEMPLATE_API IAI_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;

	
};

