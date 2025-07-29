// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_Tower.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/Interfaces/HAbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"


UBTS_Tower::UBTS_Tower()
{
	NodeName = TEXT("Service Tower");
}

void UBTS_Tower::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIPawn);
	
	if (ICombatInterface::Execute_IsDead(AIPawn))
	{
		BlackboardComp->SetValueAsEnum(FName("StateEnum"),6); // Idle
		if (ACharacter * CH = Cast<ACharacter>(AIPawn))
		{
			CH->GetCharacterMovement()->StopMovementImmediately();
		}
		if (ASC)
		{
			IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
		}
		return;
	}

	
	uint8 EnumValue = BlackboardComp->GetValueAsEnum(FName("StateEnum"));
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("TargetActor")));
	float AttackRange = BlackboardComp->GetValueAsFloat(FName("AttackRange"));
	
	if (TargetActor)
	{

		bool bTargetDead = ICombatInterface::Execute_IsDead(TargetActor);
		float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
		

		if (bTargetDead) // Target öldüyse 
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"),nullptr);
			ICombatInterface::Execute_SetTargetActor(AIPawn,nullptr);
			if (ASC)
			{
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
			}
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),6); // Idle
		}

		if (DistanceToTarget > AttackRange) // Kule rangesinden çıkmış
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"),nullptr);
			ICombatInterface::Execute_SetTargetActor(AIPawn,nullptr);
			if (ASC)
			{
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
			}
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),6); // Idle
		}

		BlackboardComp->SetValueAsEnum(FName("StateEnum"),3); // Attacking
		
		/*if (EnumValue != 3)
		{
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),3); // Attacking
		}*/
		
	}
	else
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(AIPawn);
		
		//AActor* ClosestActor = UHBlueprintFunctionLibrary::Tower_GetClosestTarget(AIPawn,ActorsToIgnore,AttackRange);
		AActor* ClosestActor = UHBlueprintFunctionLibrary::Minion_GetClosestTarget(AIPawn,ActorsToIgnore,AttackRange);

		if (ClosestActor && !ICombatInterface::Execute_IsDead(ClosestActor)) 
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"),ClosestActor);
			ICombatInterface::Execute_SetTargetActor(AIPawn,ClosestActor);
		}
	}
	
}

/**
 * TargetActor setlendimi ----- Attacking Geçir
 *
 *kulede attack range kullanılır sadece 
 *
 * TargetActor Null sa idle geçir
 */
