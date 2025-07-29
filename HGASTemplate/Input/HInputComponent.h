
// HANEDAN COMPANY

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "HGASTemplate/Input/Data_InputConfig.h"
#include "HInputComponent.generated.h"

class UData_InputConfig;
/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	/**
	 * COURSE NOTES
	 * Etkin olması için project settings - inputtan ayarlanmalı
	 */

public:
	/**
	 * So this function is a template function capable of receiving functions or function pointers and whatever
	 * that function signature is. It doesn't matter. This is a template, so we can just pass in the function
	 * we want and this function will handle it.
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(UData_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UHInputComponent::BindAbilityActions(UData_InputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for (const FData_InputConfigInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				// T = Action.InputTag)
				// Evet T diye bir veriyi ekleyebiliriz callback fonku tanımlarken Callback(T) diye tanımla ve veriye eriş
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}

