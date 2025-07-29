// Fill out your copyright notice in the Description page of Project Settings.


#include "HAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


// Sets default values
AHAIController::AHAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
}

// Called when the game starts or when spawned
void AHAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

