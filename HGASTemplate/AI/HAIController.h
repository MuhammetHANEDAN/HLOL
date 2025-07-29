// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "HAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class HGASTEMPLATE_API AHAIController : public AAIController
{
	GENERATED_BODY()

public:
	
	AHAIController();

protected:
	
	virtual void BeginPlay() override;

public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
	

	
};
