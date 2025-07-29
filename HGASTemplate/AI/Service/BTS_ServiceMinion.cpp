// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_ServiceMinion.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/Interfaces/HAbilitySystemInterface.h"

UBTS_ServiceMinion::UBTS_ServiceMinion()
{
	NodeName = "Service Minion";
}

void UBTS_ServiceMinion::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;
    
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	//ACharacter* AICharacter = Cast<ACharacter>(AIPawn); // ?
	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComp) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIPawn);
	

	if (BlackboardComp->GetValueAsEnum(FName("StateEnum")) == 0) return; // None Statesinde Pawnın ölmesi veya stun yemesi için kullanılacak

	if (ICombatInterface::Execute_IsDead(AIPawn))
	{
		BlackboardComp->SetValueAsEnum(FName("StateEnum"),0); // None
		if (ACharacter * CH = Cast<ACharacter>(AIPawn))
		{
			CH->GetCharacterMovement()->StopMovementImmediately();
		}
		return;
	}
	
	uint8 EnumValue = BlackboardComp->GetValueAsEnum(FName("StateEnum"));

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FName("TargetActor")));

	float DetectEnemyRange = BlackboardComp->GetValueAsFloat(FName("DetectEnemyRange"));

	// Middle Spline a olaan mesafe alınıp 1200 den fazla ise geri middle spline a geri döndürülebilir. ? 

	if (TargetActor)
	{
		// Target öldümü ? Targetle Mesafe Açıldımı Target etkinken

		bool bTargetDead = ICombatInterface::Execute_IsDead(TargetActor);

		float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
		float ChaseDistance = BlackboardComp->GetValueAsFloat(FName("ChaseDistance"));

		float AttackRange = BlackboardComp->GetValueAsFloat(FName("AttackRange"));
		
		if (bTargetDead || DistanceToTarget > ChaseDistance) // Target öldüyse veya çok uzaklaştıysa
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"),nullptr);
			ICombatInterface::Execute_SetTargetActor(AIPawn,nullptr);
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),4); // ReturnToLine
		}
		
		else if (DistanceToTarget > AttackRange) // Yukardakiler olmadı ama Attack rangeden çıkyısa ve attack ediyorsa
		{
			if (TargetActor->ActorHasTag(FName("Tower"))) return;
			if (ASC)
			{
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
			}
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),2); // Chase
		}
	}
	
	else
	{
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(AIPawn);
		
		AActor* ClosestActor = UHBlueprintFunctionLibrary::Minion_GetClosestTarget(AIPawn,ActorsToIgnore,DetectEnemyRange);

		if (ClosestActor && !ICombatInterface::Execute_IsDead(ClosestActor) && EnumValue != 2) // CLosest Actor var ve ölü değil ve chasede değiliz
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"),ClosestActor);
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),2); // Chase
			ICombatInterface::Execute_SetTargetActor(AIPawn,ClosestActor);
		}
		else 
		{
			if (ASC)
			{
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
			}
			if (EnumValue == 4  || EnumValue == 1) return; //? Line da veya going forwardda değilse
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),4); // ReturnToLine
			
		}
	}

}

/**
	 *  Hedef Varsa
	 *
	 *  Trace Atma
	 *  Mesafeyi ölç gerekirse GA yı durdur ve chase ye geçir
	 *
	 *  Hedef Yoksa
	 *  
	 *  Trace At - Traceden sonra hedef bulunamadıysa elleme
	 *
	 *  
	 */
