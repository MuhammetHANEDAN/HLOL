// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_Tower.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UBTS_Tower : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_Tower();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
