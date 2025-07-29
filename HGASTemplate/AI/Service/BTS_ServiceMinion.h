// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_ServiceMinion.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UBTS_ServiceMinion : public UBTService
{
	GENERATED_BODY()

	public:

	UBTS_ServiceMinion();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
