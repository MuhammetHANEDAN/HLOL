// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_JungleCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UBTS_JungleCharacter : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_JungleCharacter();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
