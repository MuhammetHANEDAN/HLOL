
#include "HCharacter.h"

#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/Components/StatusVFXHandlerComponent.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/PlayerCore/PlayerControllers/HPlayerController.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"
#include "HGASTemplate/UI/HUDS/HHUD.h"
#include "HGASTemplate/UI/WidgetComponents/BarComponent.h"
#include "Net/UnrealNetwork.h"


void AHCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetActorLostDelegate.AddLambda(
		[this]()
		{
			if(HPlayerController)
			{
				HPlayerController->TargetActor = nullptr;
			}
		});

	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&ThisClass::Delay_SetAbilitySystemComponent,2);

	/*FTimerHandle TH2;
	GetWorldTimerManager().SetTimer(TH2,this,&ThisClass::Delayed_HideLocalBar,2);*/
	
}

AHCharacter::AHCharacter()
{
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0,400,0);

	GetCharacterMovement()->bConstrainToPlane = true; // top down oyun için
	GetCharacterMovement()->bSnapToPlaneAtStart = true; // top down oyun için

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	BarComponent =CreateDefaultSubobject<UBarComponent>("Bar Component");
	BarComponent->SetupAttachment(GetRootComponent());
	
}

void AHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHCharacter,TargetActor);
}

void AHCharacter::PossessedBy(AController* NewController) // Serverde çalışıyor sadece
{
	Super::PossessedBy(NewController);
	
	// init ability actor info for server
	InitAbilityActorInfo();
	AddCharacterAbilities(); // Sadece serverde çağılıması gerektiği için, startup abilityleri ekliyoruz. Aç

	AHPlayerState* HPlayerState = GetPlayerState<AHPlayerState>();
	check(HPlayerState);
	HPlayerState->OnLevelChangedDelegate.AddLambda(
		[&](int32 Level, bool bLevelUp)
		{
			OnLevelChanged.Broadcast(Level);
		});
	
}

void AHCharacter::OnRep_PlayerState() // Clientte
{
	Super::OnRep_PlayerState();
	
	// init ability actor info for client
	InitAbilityActorInfo();

	AHPlayerState* HPlayerState = GetPlayerState<AHPlayerState>();
	check(HPlayerState);
	HPlayerState->OnLevelChangedDelegate.AddLambda(
		[&](int32 Level, bool bLevelUp)
		{
			OnLevelChanged.Broadcast(Level);
		});
}

void AHCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	if(!bHplayerControllerASInitialized)
	{
		FTimerHandle TH;

		// Bu işlem için HEM Ability Systemin initialized olmuş olması hemde HPlayer Controllerın alınmış olması gerekmekte
		// Daha iyi bir yol bulunabilir.
		GetWorldTimerManager().SetTimer(TH,this,&ThisClass::InitializeControllerAS,1,false);
		
	}
}

void AHCharacter::AddToXP_Implementation(int32 InXP)
{
	AHPlayerState* HPState = GetPlayerState<AHPlayerState>();
	check(HPState);
	IPlayerInterface::Execute_AddToXP(HPState,InXP);
}

void AHCharacter::AddToGold_Implementation(int32 InGold)
{
	AHPlayerState* HPState = GetPlayerState<AHPlayerState>();
	check(HPState);
	IPlayerInterface::Execute_AddToXP(HPState,InGold);
	
}

AActor* AHCharacter::GetTargetCharacter_Implementation()
{
	check(HPlayerController);
	return HPlayerController->TargetActor;
}

float AHCharacter::GetXPReward_Implementation()
{
	return XpLevelCurveTable.Curve.CurveTable->FindCurve(FName("Player"),FString("HCC GetXPReward_imp"))->Eval(ICombatInterface::Execute_GetPlayerLevel(this));
}

UHAttributeSet* AHCharacter::I_GetHAS_Implementation()
{
	return GetHAS();
}

int32 AHCharacter::GetGoldReward_Implementation()
{
	return 250;
}

int32 AHCharacter::GetPlayerLevel_Implementation()
{
	const AHPlayerState* HPlayerState = GetPlayerState<AHPlayerState>();
	check(HPlayerState);
	return HPlayerState->GetPlayerLevel();
}

void AHCharacter::Delay_SetAbilitySystemComponent()
{
	AHPlayerState* HPlayerState = GetPlayerState<AHPlayerState>();
	AbilitySystemComponent = HPlayerState->GetAbilitySystemComponent();
}

void AHCharacter::AbilitySystemInitialized(const UAbilitySystemComponent* ASC)
{
	InitializeControllerAS();

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHAS()->GetMoveSpeedAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
		});
	
	GetCharacterMovement()->MaxWalkSpeed = GetHAS()->GetMoveSpeed();
}

void AHCharacter::InitializeControllerAS()
{
	if(Controller == nullptr) return;
	HPlayerController = Cast<AHPlayerController>(Controller);
	if(HPlayerController)
	{
		HPlayerController->AbilitySystemComponent = AbilitySystemComponent;
		HPlayerController->AttributeSet = AttributeSet;
		HPlayerController->AbilitySystemWorks();
		bHplayerControllerASInitialized = true;
	}
}

void AHCharacter::InitAbilityActorInfo()
{
	AHPlayerState* HPlayerState = GetPlayerState<AHPlayerState>();
	check(HPlayerState);
	HPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(HPlayerState,this);
	AbilitySystemComponent = HPlayerState->GetAbilitySystemComponent();
	Cast<UHAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet(); // Delegate bağlama
	AttributeSet = HPlayerState->GetAttributeSet();

	OnAscRegistered.AddLambda(
		[this](UAbilitySystemComponent* ASC)
		{
			AbilitySystemComponent = ASC;
			AbilitySystemInitialized(AbilitySystemComponent);
		});
	
	OnAscRegistered.Broadcast(AbilitySystemComponent);

	StatusVFXHandlerComponent->InitStatusVFXHandler(AbilitySystemComponent);

	BarComponent->StartInitPlayerBarComp();

	//AbilitySystemComponent->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AHCharacter::StunTagChanged);

	HPlayerController = Cast<AHPlayerController>(Controller);
	if(HPlayerController)
	{
		AbilitySystemComponent->AbilityActorInfo.Get()->PlayerController = HPlayerController;
		if(AHHUD* HHUD = Cast<AHHUD>(HPlayerController->GetHUD()))
		{
			HHUD->InitOverlay(HPlayerController,HPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
	InitializeDefaultAttributes(); 
}

void AHCharacter::SetTargetActor(AActor* TActor)
{
	TargetActor = TActor;
	ICombatInterface::Execute_UpdateFacingTarget(this,TActor);
	//UpdateFacingTarget(TActor);
}

void AHCharacter::OnRep_TargetActor()
{
	ICombatInterface::Execute_UpdateFacingTarget(this,TargetActor);
}

void AHCharacter::ResetCharacterForRespawn()
{
	GetCharacterMovement()->MaxWalkSpeed = GetHAS()->GetMoveSpeed();
	GetHAS()->SetHealth(GetHAS()->GetMaxHealth());
}

void AHCharacter::Delayed_HideLocalBar()
{
	if(Controller && Controller->IsLocalController() && ActorHasTag(FName("Player")))
	{
		BarComponent->SetVisibility(false);
	}
}

void AHCharacter::Die(const FVector& DeathImpulse, AActor* KillerActor)
{
	if (HasAuthority() == false) return;
	Super::Die(DeathImpulse, KillerActor);
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&ThisClass::DeathToWaitRespawnZone,DeathToRespawnZoneDelay);
}

void AHCharacter::Multicast_HandleDeath(const FVector& DeathImpulse)
{
	Super::Multicast_HandleDeath(DeathImpulse);

	BarComponent->SetVisibility(false);
}

void AHCharacter::DeathToWaitRespawnZone()
{
	SetActorLocation(FVector(30360,0,-1000.f));
}

void AHCharacter::ReSpawn(const FVector& Location, const FRotator& Rotation)
{
	if (HasAuthority() == false) return;
	
	bDead = false;
	//AbilitySystemComponent->RemoveReplicatedLooseGameplayTag(FHGamePlayTags::Get().State_Dead);
	Multicast_HandleReSpawn();
	
	ResetCharacterForRespawn();

	SetActorLocation(Location);
	SetActorRotation(Rotation);

	/*
	if (bIsRadiant)
	{
		SetActorLocation(FVector(-3000,0,0));
	}
	else
	{
		SetActorLocation(FVector(3000,0,0));
	}*/

	SetCameraLocationToCharacter(); // İçinde client rpc çağırıyor
}

void AHCharacter::Multicast_HandleReSpawn_Implementation()
{
	//OnAliveDelegate.Broadcast(); ?
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap); // Projectile
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore); // Minion
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Overlap); // Skill

	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Block);

	if (ReSpawnMontage)
	{
		GetMesh()->GetAnimInstance()->Montage_Play(ReSpawnMontage);
	}
	
	bool bIsLocalPlayer = Controller && Controller->IsLocalController();

	if (!bIsLocalPlayer)
	{
		BarComponent->SetVisibility(true);
	}
	
}

void AHCharacter::SetCameraLocationToCharacter()
{
	if (HPlayerController)
	{
		HPlayerController->Client_SetCameraLocationToCharacter(GetActorLocation());
	}
}


//--------------------------------- DEbug---------------------------------------------

void AHCharacter::Debug_SetTeam(bool bRadiant)
{
	Server_SetTeam(bRadiant);
}

void AHCharacter::Server_Debug_AddGold_Implementation()
{
	AHPlayerState* PS = GetPlayerState<AHPlayerState>();

	if (PS)
	{
		IPlayerInterface::Execute_AddToGold(PS,2000);
	}
}

void AHCharacter::Server_Debug_AddXp_Implementation(float Value)
{
	AHPlayerState* PS = GetPlayerState<AHPlayerState>();

	if (PS)
	{
		IPlayerInterface::Execute_AddToXP(PS,Value);
	}
}








