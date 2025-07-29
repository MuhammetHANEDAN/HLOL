
#include "HAI_Character.h"

#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HGASTemplate/HGASTemplate.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/Actors/HJungleSpawner.h"
#include "HGASTemplate/Actors/HSpawner.h"
#include "HGASTemplate/AI/HAIController.h"
#include "HGASTemplate/Components/StatusVFXHandlerComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "HGASTemplate/UI/WidgetComponents/MinionBarComponent.h"


AHAI_Character::AHAI_Character()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UHAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UHAttributeSet>("AttributeSet");
	
	CurrentWalkSpeed = 250.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	MinionBarComponent = CreateDefaultSubobject<UMinionBarComponent>("Minion Bar Component");
	MinionBarComponent->SetupAttachment(GetRootComponent());

	
}

void AHAI_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;

	HAIController = Cast<AHAIController>(NewController);

	if (HAIController && BehaviorTree)
	{
		HAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

		//HAIController->GetBlackboardComponent()

		if (bIsMinion)
		{
			HAIController->GetBlackboardComponent()->SetValueAsVector(FName("EndTargetLocation"),EndTargetLocation);
			HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),4); // Return To Line
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("ChaseDistance"),ChaseDistance);
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackTetherDistance"),AttackTetherDistance);
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("DetectEnemyRange"),DetectEnemyRange);
			
			BBInitialized(); // Middle Spline ı BP de almak için
		}

		if (bIsJungleMob || bIsDragon)
		{
			HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),6); // Idle
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("ChaseDistance"),ChaseDistance);
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackTetherDistance"),AttackTetherDistance);

			HAIController->GetBlackboardComponent()->SetValueAsVector(FName("StartLocation"),GetActorLocation());
			HAIController->GetBlackboardComponent()->SetValueAsRotator(FName("StartRotation"),GetActorRotation());
			
		}

		if (bIsTower)
		{
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackRange"),TowerAttackRange);
			HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),6); // Idle
		}
		
		HAIController->RunBehaviorTree(BehaviorTree);
	}
	
}

void AHAI_Character::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
	
	InitAbilityActorInfo();
	
}

void AHAI_Character::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UHAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet(); // Delegate bağlama

	//AbilitySystemComponent->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHAI_Character::StunTagChanged);

	if(HasAuthority())
	{
		InitializeDefaultAttributes();
		AddCharacterAbilities();
	}

	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemInitialized(AbilitySystemComponent);
	
	StatusVFXHandlerComponent->InitStatusVFXHandler(AbilitySystemComponent);
	

	MinionBarComponent->InitMinionBarComp(AbilitySystemComponent,AttributeSet);
	
}

void AHAI_Character::InitializeDefaultAttributes() const
{
	ApplyGamePlayEffectToSelf(DefaultAttributes,Level);
	ApplyGamePlayEffectToSelf(DefaultVitalAttributes,Level);
}

int32 AHAI_Character::GetPlayerLevel_Implementation()
{
	return Level;
}

void AHAI_Character::Die(const FVector& DeathImpulse, AActor* KillerActor)
{
	if (bIsTower)
	{
		if (HasAuthority() == false) return;
	
		bDead = true;
		if (AbilitySystemComponent)
		{
			IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
		}

		Multicast_TowerExplosion();
		Destroy();
		
	}
	else
	{
		Super::Die(DeathImpulse, KillerActor);
	
		HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),0); // State None geçti
		SetLifeSpan(4);
	
		if (Spawner.Get())
		{
			Spawner.Get()->SpawnedCharacterDead();
		}

	
		HAIController->BrainComponent->StopLogic(FString("Death"));
		GetCharacterMovement()->DisableMovement();
	}
	
	
}

float AHAI_Character::GetXPReward_Implementation()
{
	return XpLevelCurveTable.GetValueAtLevel(Level);
}

UHAttributeSet* AHAI_Character::I_GetHAS_Implementation()
{
	return GetHAS();
}

int32 AHAI_Character::GetGoldReward_Implementation()
{
	if (bIsTower)
	{
		return 250;
	}
	
	if (bIsDragon)
	{
		return 500;
	}

	if (bIsJungleMob)
	{
		return 150;
	}
	
	return 60; // Minyon ise
}

void AHAI_Character::SetTargetActor_Implementation(AActor* TActor)
{
	TargetActor = TActor;

	ICombatInterface::Execute_UpdateFacingTarget(this,TActor);
	
	if (TActor)
	{
		ICombatInterface::Execute_UpdateFacingTargetLocation(this,TActor->GetActorLocation());
	}

	if (HAIController)  
	{
		HAIController->GetBlackboardComponent()->SetValueAsObject(FName("TargetActor"),TActor);
	}
}

AActor* AHAI_Character::GetTargetCharacter_Implementation()
{
	return TargetActor.Get();
}

void AHAI_Character::H_HighlightActor_Implementation(bool bEnemy)
{
	Super::H_HighlightActor_Implementation(bEnemy);

	if (bIsTower)
	{
		H_SetRenderCustomDepth(true);
		
		if(bEnemy)
		{
			H_SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
		}
		else
		{
			H_SetCustomDepthStencilValue(CUSTOM_DEPTH_GREEN);
		}
	}
}

void AHAI_Character::H_UnHighlightActor_Implementation()
{
	Super::H_UnHighlightActor_Implementation();

	if (bIsTower)
	{
		H_SetRenderCustomDepth(false);
	}
}

UAbilitySystemComponent* AHAI_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHAI_Character::AbilitySystemInitialized(const UAbilitySystemComponent* ASC)
{
	float AttackRange = UHBlueprintFunctionLibrary::GetAttributeValueByTag(this,FHGamePlayTags::Get().Attributes_AttackRange);

	if (HasAuthority())
	{
		if (bIsTower) return;
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				check(HAIController);
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
				HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),0); // None
				//ICombatInterface::Execute_SetTargetActor(this,nullptr);
				GetCharacterMovement()->SetAvoidanceEnabled(false);
				
			}
			else
			{
				check(HAIController);
				if (bIsMinion)
				{
					HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),4); // Return To Line
				}
				else if (bIsJungleMob)
				{
					HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),6); // Idle
				}
				else if (bIsDragon)
				{
					HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),6); // Idle
				}
				
				GetCharacterMovement()->SetAvoidanceEnabled(true);
			}
		});

		AbilitySystemComponent->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Knockback, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				check(HAIController);
				IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
				HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),0); // None
				//ICombatInterface::Execute_SetTargetActor(this,nullptr);
				GetCharacterMovement()->SetAvoidanceEnabled(false);
			}
			else
			{
				check(HAIController);
				if (bIsMinion)
				{
					HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),4); // Return To Line
				}
				else if (bIsJungleMob)
				{
					HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),6); // Idle
				}
				else if (bIsDragon)
				{
					HAIController->GetBlackboardComponent()->SetValueAsEnum(FName("StateEnum"),6); // Idle
				}
				GetCharacterMovement()->SetAvoidanceEnabled(true);
			}
		});
		
		if (HAIController)
		{
			HAIController->GetBlackboardComponent()->SetValueAsFloat(FName("AttackRange"),AttackRange);
		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("HAI Controller Not Valid Sıkıntı"));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(1,3,FColor::Red,"HAI Controller Not Valid Sıkıntı");
			}
		}
	}
	
}
