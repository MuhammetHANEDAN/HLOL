// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HGameplayAbility.h"
#include "HPassiveGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHPassiveGameplayAbility : public UHGameplayAbility
{
	GENERATED_BODY()

public:

	//?
	/** Karakterin pasifi olmayan, Q W E R skillerinden olan Pasif bir Yetenek olup olmadığını belirler
	 * Direk Fire Yemeyen Pasif Skilldir. Leveli Arttıkça Kattığı Etkiler Artar.
	 * 
	 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "AProps")
	bool bPassiveSlotAbility = false;


	
};
