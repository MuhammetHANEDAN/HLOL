
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHUserWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();









	
	
};

