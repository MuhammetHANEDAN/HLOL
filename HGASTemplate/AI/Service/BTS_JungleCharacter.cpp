// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_JungleCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/Interfaces/HAbilitySystemInterface.h"
#include "AbilitySystemComponent.h"

class UAbilitySystemComponent;

UBTS_JungleCharacter::UBTS_JungleCharacter()
{
	NodeName = "Service Jungle Character";
}

void UBTS_JungleCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
	

	if (TargetActor)
	{
		// Target öldümü ? Targetle Mesafe Açıldımı Target etkinken

		bool bTargetDead = ICombatInterface::Execute_IsDead(TargetActor);

		float DistanceToTarget = AIPawn->GetDistanceTo(TargetActor);
		float ChaseDistance = BlackboardComp->GetValueAsFloat(FName("ChaseDistance"));

		FVector StartLocation = BlackboardComp->GetValueAsVector(FName("StartLocation"));
		
		float DistanceToStartLocation = (StartLocation-AIPawn->GetActorLocation()).Length();

		float AttackRange = BlackboardComp->GetValueAsFloat(FName("AttackRange"));
		
		if (bTargetDead) // Target öldüyse  uzaklaştıysa
		{
			BlackboardComp->SetValueAsObject(FName("TargetActor"),nullptr);
			ICombatInterface::Execute_SetTargetActor(AIPawn,nullptr);
			if (ASC)
			{
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
			}
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),5); // ReturnToStartLoc
		}

		if (EnumValue != 2 && EnumValue != 3) // Chaselemiyorsa hali hazırda Attack etmiyor ise hali hazırda
		{
			BlackboardComp->SetValueAsEnum(FName("StateEnum"),2); // chaase
		}

		if (EnumValue == 2) // Chasede iken
		{
			if (DistanceToStartLocation > ChaseDistance)
			{
				BlackboardComp->SetValueAsObject(FName("TargetActor"),nullptr);
				ICombatInterface::Execute_SetTargetActor(AIPawn,nullptr);
				if (ASC)
				{
					IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
				}
				BlackboardComp->SetValueAsEnum(FName("StateEnum"),5); // ReturnToStartLoc
			}
		}

		if (EnumValue == 3) // Attacking de iken
		{
			if (DistanceToTarget > AttackRange) // Yukardakiler olmadı ama Attack rangeden çıkyısa ve attack ediyorsa
			{
				if (ASC)
				{
					IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
				}
				BlackboardComp->SetValueAsEnum(FName("StateEnum"),2); // Chase
			}
		}
	}
	
	else if (EnumValue != 5 && EnumValue != 6) // Hali hazırda returnloc a dönmüyorsa ve idle değilse
	{
		BlackboardComp->SetValueAsObject(FName("TargetActor"),nullptr);
		ICombatInterface::Execute_SetTargetActor(AIPawn,nullptr);
		if (ASC)
		{
			IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(ASC);
		}
		BlackboardComp->SetValueAsEnum(FName("StateEnum"),5); // ReturnToStartLoc
	}
	
}



