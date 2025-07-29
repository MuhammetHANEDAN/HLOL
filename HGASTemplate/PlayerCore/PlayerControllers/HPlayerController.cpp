
#include "HPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HGASTemplate/AbilitySystem/HAbilitySystemComponent.h"
#include "HGASTemplate/AbilitySystem/HAttributeSet.h"
#include "HGASTemplate/AbilitySystem/Abilities/NormalAttackAbility.h"
#include "HGASTemplate/Camera/MobaCameraActor.h"
#include "HGASTemplate/Characters/HCharacter.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/GameCore/HGameInstance.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"
#include "HGASTemplate/GeneralBlueprintFunctionLibrarys/HBlueprintFunctionLibrary.h"
#include "HGASTemplate/Input/HInputComponent.h"
#include "HGASTemplate/UI/WidgetComponents/DamageTextWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "Kismet/KismetMathLibrary.h"


AHPlayerController::AHPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	bAutoManageActiveCameraTarget = false;
}

void AHPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	if(bAutoRunning)
	{
		AutoRun();
	}

	if(bAutoRunningToTarget)
	{
		AutoRunToTarget();
	}

	if(bAutoRunningToTarget && !bNeedUpdateTargetNavPath)
	{
		UpdateTargetNavPathThresholdTrack += DeltaTime;
		if(UpdateTargetNavPathThresholdTrack >= UpdateTargetNavPathThreshold)
		{
			UpdateTargetNavPathThresholdTrack =0;
			bNeedUpdateTargetNavPath = true;
		}
	}

	if(bNormalAttackOnGoing)
	{
		
		if (TargetActor == nullptr || ICombatInterface::Execute_IsDead(TargetActor))
		{
			bNormalAttackOnGoing = false;
			GetHASC()->EndNormalAttackAbility();
		}
		// Saldırı komutunda iken düşman karakter menzilden uzaklaştı
		if(TargetActor && GetPawn()->GetDistanceTo(TargetActor) >= (AttackRange + TethetDistanceForNormalAttack))
		{
			// Anim oynuyorsa animin damage atan kısmına kadar bekletmeyi sağlamalıyız
			bNormalAttackOnGoing = false;

			GetHASC()->EndNormalAttackAbility();
			EnableAutoMovementToTarget();

			/*UNormalAttackAbility* NAbility = Cast<UNormalAttackAbility>(GetHASC()->GetNormalAttackAbility());
			NAbility->bEndNormalAttackAbilityWhenAttackAnimEnd = true;*/
		}
		
	}

	if(MobaCameraActor && bCanMobaCameraMove)
	{
		GetMousePosition(CurrentMouseLocX,CurrentMouseLocY);
		MobaCameraMove(DeltaTime);
	}
	
}

void AHPlayerController::AnimEvent_HandleNormalAttackAbility_Implementation() // Anim bitti şu anki durum ne
{
	
}

void AHPlayerController::ContinueNormalAttack_Implementation()
{
	
	if (ControlledPawn == nullptr)
	{
		return;
	}

	if (TargetActor == nullptr)
	{
		return;
	}
	
	const float DistanceToDestination = ControlledPawn->GetDistanceTo(TargetActor);
	
	if(DistanceToDestination <= AttackRange)
	{
		bAutoRunning = false;
		
		bAutoRunningToTarget = false;
		UpdateTargetNavPathThresholdTrack = 0;
		bNeedUpdateTargetNavPath = false;

		bNormalAttackOnGoing = true;
		GetHASC()->ActivateNormalAttackAbility();
	}
	else
	{
		bAutoRunningToTarget = true;
	}
}

void AHPlayerController::GameStarted_Implementation()
{
	Client_GameStarted();
	Client_SetCameraLocationToCharacter(GetPawn()->GetActorLocation());
}

AActor* AHPlayerController::GetTargetActor_Implementation()
{
	return TargetActor;
}

void AHPlayerController::I_ShowKillWidget_Implementation(const FGameplayTag& KillerTag, const FGameplayTag& VictimTag)
{
	Client_ShowKillWidget(KillerTag,VictimTag);
}

void AHPlayerController::I_ResetHPlayerControllerMovementAndTarget_Implementation()
{
	ResetHPlayerControllerMovementAndTarget();
}

void AHPlayerController::I_SetCameraLocationToPawn_Implementation()
{
	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&ThisClass::SetCameraLocationToPawn,0.1f);
}

void AHPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHPlayerController,bIsRadiant);
}

void AHPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	FString MapName = UGameplayStatics::GetCurrentLevelName(this);
	
	if (MapName != TEXT("TestMap"))
	{
		return;
	}

	if (HasAuthority())
	{
		HLOG("posseslendi, client kamera setlenmesi lazım");
		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH,this,&ThisClass::Client_DelayedSetviewTarget,0.2f);

		//Client_DelayedSetviewTarget();
	}
}

void AHPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FString MapName = UGameplayStatics::GetCurrentLevelName(this);
	UE_LOG(LogTemp, Warning, TEXT("Current Level: %s"), *MapName);

	if (MapName != TEXT("TestMap"))
	{
		return;
	}
	
	
	check(HMappingContext); 
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(HMappingContext,0); 
	}
	
	bShowMouseCursor = true;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	
	if(IsLocalController())
	{
		UHGameInstance* HGameInstance = GetGameInstance<UHGameInstance>();
		Server_SetIsRadiant(HGameInstance->bIsRadiantPlayer);
		Server_SetHeroTag(HGameInstance->HeroTag);

		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH,this,&ThisClass::InitializeControlledCharacterTeamAfterDelay,1.5);

		if (StartWidgetClass)
		{
			StartWidget = CreateWidget<UUserWidget>(this, StartWidgetClass);
			StartWidget->AddToViewport();
		}
		
	}
	
	if(IsLocalController() && MobaCameraActorClass)
	{
		GetViewportSize(ViewportSizeX,ViewportSizeY);
		
		/*FActorSpawnParameters Sparams;
		Sparams.Owner = this;
		const FVector SpawnLoc = FVector(0,0,300);
		const FRotator SpawnRot = FRotator::ZeroRotator;*/
		//MobaCameraActor = GetWorld()->SpawnActor<AMobaCameraActor>(MobaCameraActorClass,SpawnLoc,SpawnRot,Sparams);

		FTransform Transform;
		MobaCameraActor = GetWorld()->SpawnActorDeferred<AMobaCameraActor>(MobaCameraActorClass,Transform,this);
		MobaCameraActor->PC = this;
		MobaCameraActor->FinishSpawning(Transform);

		/*#if WITH_EDITOR
		
		SetViewTarget(MobaCameraActor);
	
		FTimerHandle TH;
		GetWorldTimerManager().SetTimer(TH,this,&ThisClass::DelayedSetviewTarget,2,false);
		
		#endif*/
		
	}
	
}

void AHPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UHInputComponent* HInputComponent = CastChecked<UHInputComponent>(InputComponent);
	

	// Abilityler İçin Config
	HInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);

	HInputComponent->BindAction(MouseScrollInputAction,ETriggerEvent::Triggered,this,&AHPlayerController::MouseScroll);

	HInputComponent->BindAction(CancelAbilitiesInputAction,ETriggerEvent::Started,this,&AHPlayerController::Input_CancelAbilities);
	
}

void AHPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (ICombatInterface::Execute_IsDead(GetPawn())) return;
	
	if(InputTag.MatchesTagExact(FHGamePlayTags::Get().InputTag_RMB))
	{
		bAutoRunning = false; // Short press olup olmadığına şu an bilmiyoruz
	}
	if (GetHASC() && GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Debuff_Stun)) return;
	if (GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().State_Dead)) return;
	if (GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Debuff_Knockback)) return;
	
	GetHASC()->AbilityInputTagPressed(InputTag);
}

void AHPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	/*if (GetASC() && GetASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Player_Block_InputReleased))
	{
		return;
	}*/

	if (ICombatInterface::Execute_IsDead(GetPawn())) return;

	if (GetHASC() && GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Debuff_Stun)) return;
	if (GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().State_Dead)) return;
	if (GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Debuff_Knockback)) return;
	
	if(!InputTag.MatchesTagExact(FHGamePlayTags::Get().InputTag_RMB)) // released olan RMB değilse
	{
		if(GetHASC()) GetHASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if(GetHASC()) GetHASC()->AbilityInputTagReleased(InputTag);

	ControlledPawn = GetPawn();

	// Düşman karaktere tıklanıldı
	if(ControlledPawn && ThisActor.Get() && UHBlueprintFunctionLibrary::IsEnemy(GetPawn(),ThisActor.Get()))
	{
		bAutoRunning = false;
		
		// Şimdi düşman karaktere doğru path alınıp gidilecek
		TargetActor = ThisActor.Get();

		// Servera targetactor bilgisini vermeliyizki GA ve GE ler düzgün çalışabilsin
		// Warp Target - FVector - FacingTarget olarak servere gidiyor, clientlere dağılıyor
		Server_SetTargetActor(TargetActor);
		
		TargetNavPath =UNavigationSystemV1::FindPathToActorSynchronously(this,ControlledPawn->GetActorLocation(),TargetActor,20);
	
		if(TargetNavPath)
		{
			Spline->ClearSplinePoints(); // Önce spline pointleri clearlıyoruzki önceden setlenmişse o değerler silinsin
			for(const FVector& PointLoc : TargetNavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
			}
			
			/*if(TargetNavPath->PathPoints.Num() > 0)
			{
				CachedDestination = TargetNavPath->PathPoints[TargetNavPath->PathPoints.Num()-1]; // Kendi Projelerimizde belki kullanmayabiliriz
			}*/
		}
		
		bAutoRunningToTarget = true;
	}
	
	else if(FollowTime <= ShortPressThreshold && ControlledPawn) // Oto Movement başlıyor
	{
		// Bir yere tıklanılmış oto movement pathfind ile başlatılmalı
		//FollowTime = 0.f;

		// Eğer bir hedefe normal attack ability aktif ise şimdi iptal edilmelidir.
		GetHASC()->EndNormalAttackAbility();

		bNormalAttackOnGoing = false;

		Server_SetTargetActor(nullptr);
		TargetActor = nullptr;
		
		bAutoRunningToTarget = false;
		UpdateTargetNavPathThresholdTrack = 0;
		bNeedUpdateTargetNavPath = false;
			
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination);
		if(NavPath)
		{
			Spline->ClearSplinePoints(); // Önce spline pointleri clearlıyoruzki önceden setlenmişse o değerler silinsin
			for(const FVector& PointLoc : NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
			}
			if(NavPath->PathPoints.Num() > 0)
			{
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num()-1]; // Kendi Projelerimizde belki kullanmayabiliriz
			}
			bAutoRunning = true;
		}

		if(ClickNiagaraSystem)
		{
			// Click Niagara
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
		}
	}
	
	FollowTime = 0.f;
}

void AHPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (ICombatInterface::Execute_IsDead(GetPawn())) return;
	
	if (GetHASC() && GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	
	if (GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().State_Dead)) return;

	if (GetHASC() && GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Debuff_Stun)) return;
	
	if (GetHASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Debuff_Knockback)) return;
	
	if(!InputTag.MatchesTagExact(FHGamePlayTags::Get().InputTag_RMB)) // RMB basmıyoruz 
	{
		if(GetHASC())
		{
			GetHASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}

	//TODO endnormalattackability ? karar ver

	FollowTime += GetWorld()->GetDeltaSeconds(); // RMB basıldı ve basılmaya devam ediyor bu arada geçen süre ne kadar
	if(CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
		
	ControlledPawn = GetPawn();
	if(ControlledPawn)
	{
		const FVector Direction = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(Direction);
	}
}

void AHPlayerController::MouseScroll(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>(); 

	if (MobaCameraActor && MobaCameraActor->CameraBoom) 
	{
		
		float NewArmLength = FMath::Clamp(MobaCameraActor->CameraBoom->TargetArmLength - ZoomValue * ZoomSpeed, MinZoom, MaxZoom);
		MobaCameraActor->CameraBoom->TargetArmLength = NewArmLength;
	}
}

void AHPlayerController::Input_CancelAbilities(const FInputActionValue& Value)
{
	bool bPressed = Value.Get<bool>();

	if (bPressed && AbilitySystemComponent)
	{
		IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
		
		bAutoRunning = false;
		bAutoRunningToTarget = false;
		bNormalAttackOnGoing = false;
				
		Server_SetTargetActor(nullptr);
		TargetActor = nullptr;
				
		UpdateTargetNavPathThresholdTrack = 0;
		bNeedUpdateTargetNavPath = false;
	}
}

void AHPlayerController::AutoRun()
{
	ControlledPawn = GetPawn();
	if(ControlledPawn)
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World); // Locationun tanjantı
		ControlledPawn->AddMovementInput(DirectionOnSpline);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AHPlayerController::AutoRunToTarget()
{
	ControlledPawn = GetPawn();
	if(ControlledPawn == nullptr || TargetActor == nullptr || ICombatInterface::Execute_IsDead(TargetActor))
	{
		bAutoRunningToTarget = false;
		TargetActor = nullptr;
		return;
	}

	if(bNeedUpdateTargetNavPath)
	{
		TargetNavPath =UNavigationSystemV1::FindPathToActorSynchronously(this,ControlledPawn->GetActorLocation(),TargetActor,20);
		bNeedUpdateTargetNavPath = false;
	}

	if(TargetNavPath && TargetNavPath->PathPoints.Num() >0) 
	{
		Spline->ClearSplinePoints(true);
		for (auto& Element : TargetNavPath->PathPoints)
		{
			Spline->AddSplinePoint(Element,ESplineCoordinateSpace::World);
		}
	}

	const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
	const FVector DirectionOnSpline = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World); // Locationun tanjantı
	ControlledPawn->AddMovementInput(DirectionOnSpline);

	//UKismetSystemLibrary::DrawDebugSphere(this,LocationOnSpline,25);
	//UKismetSystemLibrary::DrawDebugArrow(this,LocationOnSpline,LocationOnSpline+(DirectionOnSpline*100),10,FLinearColor::Red);
	
	const float DistanceToDestination = ControlledPawn->GetDistanceTo(TargetActor);
	if(DistanceToDestination <= AttackRange)
	{
		bAutoRunning = false;
		
		bAutoRunningToTarget = false;
		UpdateTargetNavPathThresholdTrack = 0;
		bNeedUpdateTargetNavPath = false;

		bNormalAttackOnGoing = true;
		GetHASC()->ActivateNormalAttackAbility();
	}

	//UE_LOG(LogTemp,Error,TEXT("distance to target = %f"),DistanceToDestination);
	
}

void AHPlayerController::AbilitySystemWorks()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHAS()->GetAttackRangeAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			AttackRange = Data.NewValue;
		});
	AttackRange = GetHAS()->GetAttackRange();

	AbilitySystemComponent->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				bAutoRunning = false;
				bAutoRunningToTarget = false;
				bNormalAttackOnGoing = false;
				
				Server_SetTargetActor(nullptr);
				TargetActor = nullptr;
				
				UpdateTargetNavPathThresholdTrack = 0;
				bNeedUpdateTargetNavPath = false;
			}
		});

	AbilitySystemComponent->RegisterGameplayTagEvent(FHGamePlayTags::Get().Debuff_Knockback, EGameplayTagEventType::NewOrRemoved).AddWeakLambda(this,
		[this](const FGameplayTag& CallBackTag, int32 NewCount)
		{
			if (NewCount > 0)
			{
				bAutoRunning = false;
				bAutoRunningToTarget = false;
				bNormalAttackOnGoing = false;
				
				Server_SetTargetActor(nullptr);
				TargetActor = nullptr;
				
				UpdateTargetNavPathThresholdTrack = 0;
				bNeedUpdateTargetNavPath = false;
			}
		});
}

void AHPlayerController::DisableAutoMovementToTarget()
{
	GetHASC()->EndNormalAttackAbility();

	Server_SetTargetActor(nullptr);
	TargetActor = nullptr;
		
	
	UpdateTargetNavPathThresholdTrack = 0;
	bNeedUpdateTargetNavPath = false;
	bAutoRunningToTarget = false;
}

void AHPlayerController::EnableAutoMovementToTarget()
{
	if(TargetActor == nullptr || ICombatInterface::Execute_IsDead(TargetActor)) return;
	
	UpdateTargetNavPathThresholdTrack = 0;
	bNeedUpdateTargetNavPath = true;
	bAutoRunningToTarget = true;
	
}

UHAbilitySystemComponent* AHPlayerController::GetHASC() const
{
	if(AbilitySystemComponent)
	{
		return Cast<UHAbilitySystemComponent>(AbilitySystemComponent);
	}
	return nullptr;
}

UHAttributeSet* AHPlayerController::GetHAS() const
{
	if(AttributeSet )
	{
		return Cast<UHAttributeSet>(AttributeSet);
	}
	return nullptr;
}

void AHPlayerController::CursorTrace()
{

	/*if (GetASC() && GetASC()->HasMatchingGameplayTag(FHGamePlayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->UnHighlightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}*/
	
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	if (ThisActor == GetPawn())
	{
		

		FVector Start = FVector(0,0,0);
		FRotator Rotation = FRotator(0,0,0);
		
		GetPlayerViewPoint(Start, Rotation);

		FRotator DesiredRot = UKismetMathLibrary::FindLookAtRotation(Start,CursorHit.TraceEnd);
		FVector Forward = DesiredRot.Vector();

		FVector End = Start + Forward * 10000;
		
		
		FHitResult SecondaryHit;

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(GetPawn()); 
		Params.bTraceComplex = false;

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			SecondaryHit,
			Start,
			End,
			ECC_Visibility,
			Params
		);

		if (bHit && SecondaryHit.GetActor())
		{
			ThisActor = SecondaryHit.GetActor();
		}

		if(LastActor != ThisActor)
		{
			if(LastActor && LastActor->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_H_UnHighlightActor(LastActor);
			}
			
			if(ThisActor)
			{
				//ThisActor->Execute_HighlightActor(ThisActor.GetObject(),bIsRadiant == ThisActor->Execute_IsRadiant(ThisActor.GetObject()));
				bool bEnemy = UHBlueprintFunctionLibrary::IsEnemy(GetPawn(),ThisActor);
				if (ThisActor->Implements<UCombatInterface>())
				{
					ICombatInterface::Execute_H_HighlightActor(ThisActor,bEnemy);
				}
			
			}
		}

		if(ThisActor)
		{
			//ThisActor->Execute_HighlightActor(ThisActor.GetObject(),bIsRadiant == ThisActor->Execute_IsRadiant(ThisActor.GetObject()));
			bool bEnemy = UHBlueprintFunctionLibrary::IsEnemy(GetPawn(),ThisActor);
			if (ThisActor->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_H_HighlightActor(ThisActor,bEnemy);
			}
			
		}

		return;
		
	}
	
	if(LastActor != ThisActor)
	{
		if(LastActor && LastActor->Implements<UCombatInterface>())
		{
			ICombatInterface::Execute_H_UnHighlightActor(LastActor);
		}
		
		if (ThisActor)
		{
			if(ThisActor == GetPawn()) return;
		}
		
		
		if(ThisActor)
		{
			//ThisActor->Execute_HighlightActor(ThisActor.GetObject(),bIsRadiant == ThisActor->Execute_IsRadiant(ThisActor.GetObject()));
			bool bEnemy = UHBlueprintFunctionLibrary::IsEnemy(GetPawn(),ThisActor);
			if (ThisActor->Implements<UCombatInterface>())
			{
				ICombatInterface::Execute_H_HighlightActor(ThisActor,bEnemy);
			}
			
		}
	}
}

void AHPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	
}

void AHPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	
}

void AHPlayerController::MobaCameraMove(float DeltaTime)
{
	const FVector Forward = MobaCameraActor->ArrowForward.Get()->GetForwardVector();
	const FVector Right = MobaCameraActor->ArrowRight.Get()->GetForwardVector();
	
	// Right
	if(ViewportSizeX - CurrentMouseLocX <= ScrollCameraThreshold)
	{
		FVector DeltaLoc = Right * MouseSpeed * DeltaTime;
		MobaCameraActor->AddActorWorldOffset(DeltaLoc);
	}
	//Left
	if(CurrentMouseLocX <= ScrollCameraThreshold)
	{
		FVector DeltaLoc = Right * -1 * MouseSpeed * DeltaTime;
		MobaCameraActor->AddActorWorldOffset(DeltaLoc);
	}
	//Forward
	if(ViewportSizeY - CurrentMouseLocY <= ScrollCameraThreshold)
	{
		FVector DeltaLoc = Forward * -MouseSpeed * DeltaTime;
		MobaCameraActor->AddActorWorldOffset(DeltaLoc);
	}
	//Backward
	if(CurrentMouseLocY <= ScrollCameraThreshold)
	{
		FVector DeltaLoc = Forward  * MouseSpeed * DeltaTime;
		MobaCameraActor->AddActorWorldOffset(DeltaLoc);
	} 
	
}

void AHPlayerController::Client_SetCameraLocationToCharacter_Implementation(const FVector_NetQuantize& LocVector)
{
	/*if (MobaCameraActor && GetPawn())
	{
		FVector CurrentCameraLoc = MobaCameraActor->GetActorLocation();
		CurrentCameraLoc.X = LocVector.X;
		MobaCameraActor->SetActorLocation(CurrentCameraLoc);
	}*/

	FTimerHandle THandle;
	GetWorldTimerManager().SetTimer(THandle,this,&ThisClass::SetCameraLocationToPawn,0.15f);
	
}

void AHPlayerController::InitializeControlledCharacterTeamAfterDelay()
{
	if(GetPawn() && GetPawn()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SetIsRadiant(GetPawn(),bIsRadiant);
	}
}

void AHPlayerController::DelayedSetviewTarget()
{
	SetViewTarget(MobaCameraActor);
	bCanMobaCameraMove = true;
}

void AHPlayerController::Client_GameStarted_Implementation()
{
	DelayedSetviewTarget();
	if (StartWidget)
	{
		StartWidget->RemoveFromParent();
	}
}

void AHPlayerController::Client_DelayedSetviewTarget_Implementation()
{
	DelayedSetviewTarget();

	FVector CameraLoc = MobaCameraActor->GetActorLocation();
	CameraLoc.X = GetPawn()->GetActorLocation().X;
	CameraLoc.Y = GetPawn()->GetActorLocation().Y;
	MobaCameraActor->SetActorLocation(CameraLoc);
}

void AHPlayerController::ResetHPlayerControllerMovementAndTarget()
{
	IHAbilitySystemInterface::Execute_I_CancelAllActionAbilities(AbilitySystemComponent);
		
	bAutoRunning = false;
	bAutoRunningToTarget = false;
	bNormalAttackOnGoing = false;
				
	Server_SetTargetActor(nullptr);
	TargetActor = nullptr;
				
	UpdateTargetNavPathThresholdTrack = 0;
	bNeedUpdateTargetNavPath = false;
}

void AHPlayerController::SetCameraLocationToPawn()
{
	FVector CameraLoc = MobaCameraActor->GetActorLocation();
	CameraLoc.X = GetPawn()->GetActorLocation().X;
	CameraLoc.Y = GetPawn()->GetActorLocation().Y;
	MobaCameraActor->SetActorLocation(CameraLoc);
}

void AHPlayerController::Client_ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextWidgetComponent* DamageText = NewObject<UDamageTextWidgetComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform, FName("DamageIndicatorSocket"));
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount,bBlockedHit,bCriticalHit);
	}
}

void AHPlayerController::Server_SetIsRadiant_Implementation(bool IsRadiant)
{
	bIsRadiant = IsRadiant;
	
	if(AHCharacter* HC = Cast<AHCharacter>(GetPawn()))
	{
		HC->Server_SetTeam(bIsRadiant);
	}
	
}

void AHPlayerController::Server_SetHeroTag_Implementation(FGameplayTag Tag)
{
	HeroTag = Tag;
}

void AHPlayerController::Server_SetTargetActor_Implementation(AActor* TActor)
{
	TargetActor = TActor;
	if(AHCharacter* HC = Cast<AHCharacter>(GetPawn()))
	{
		HC->SetTargetActor(TActor);
	}
}

void AHPlayerController::Client_ShowKillWidget_Implementation(const FGameplayTag& KillerTag,
	const FGameplayTag& VictimTag)
{
	ShowKillWidget(KillerTag,VictimTag);
}

