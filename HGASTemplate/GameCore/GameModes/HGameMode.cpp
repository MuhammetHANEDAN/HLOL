
// HANEDAN COMPANY

#include "HGameMode.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "GameFramework/PlayerState.h"
#include "HGASTemplate/DEBUG/HDebugTypes.h"
#include "HGASTemplate/Interfaces/CombatInterface.h"
#include "HGASTemplate/PlayerCore/PlayerControllers/HPlayerController.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"


void AHGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TH;
	GetWorldTimerManager().SetTimer(TH,this,&AHGameModeBase::StartGame,DelayToStart,false);
	
}

void AHGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GameStarted)
	{
		GameTime += DeltaSeconds;
	}
	
}

void AHGameModeBase::StartGame()
{
	GameStarted = true;

	for (auto& Element : GetActivePlayerStates())
	{
		IPlayerInterface::Execute_InitializeGameTime(Element,GameTime);
	}

	for (auto& Element : GetActivePlayerControllers())
	{
		IPlayerInterface::Execute_GameStarted(Element);
	}
}

TArray<APlayerController*> AHGameModeBase::GetActivePlayerControllers()
{
	TArray<APlayerController*> Array;

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (IsValid(PC))
		{
			Array.AddUnique(PC);
		}
	}

	return Array;
}

TArray<APlayerState*> AHGameModeBase::GetActivePlayerStates()
{
	TArray<APlayerState*> Array;

	for (APlayerState* PlayerState : GameState->PlayerArray)
	{
		if (IsValid(PlayerState))
		{
			Array.Add(PlayerState);
		}
	}

	return Array;
}

void AHGameModeBase::RestartPlayer(AController* NewPlayer)
{
	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;
	
	RespawnDelegate.BindUFunction(this, "RestartPlayerDelayed", NewPlayer);
    
	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle,    
		RespawnDelegate,       
		3,                 
		false,               
		-1.0f               
	);
}

void AHGameModeBase::RestartPlayerDelayed(AController* NewPlayer)
{
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	AHPlayerController* PC = Cast<AHPlayerController>(NewPlayer);
	bool bIsRadiant = PC->bIsRadiant;

	AActor* StartSpot = FindHPlayerStartForTeam(bIsRadiant);

	// If a start spot wasn't found,
	if (StartSpot == nullptr)
	{
		// Check for a previously assigned spot
		if (NewPlayer->StartSpot != nullptr)
		{
			StartSpot = NewPlayer->StartSpot.Get();
			UE_LOG(LogGameMode, Warning, TEXT("RestartPlayer: Player start not found, using last start spot"));
		}	
	}

	RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
}

void AHGameModeBase::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	//Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);
	if (NewPlayer == nullptr || NewPlayer->IsPendingKillPending())
	{
		return;
	}

	if (!StartSpot)
	{
		UE_LOG(LogGameMode, Warning, TEXT("RestartPlayerAtPlayerStart: Player start not found"));
		return;
	}

	FRotator SpawnRotation = StartSpot->GetActorRotation();

	UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart %s"), (NewPlayer && NewPlayer->PlayerState) ? *NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown"));

	if (MustSpectate(Cast<APlayerController>(NewPlayer)))
	{
		UE_LOG(LogGameMode, Verbose, TEXT("RestartPlayerAtPlayerStart: Tried to restart a spectator-only player!"));
		return;
	}

	if (NewPlayer->GetPawn() != nullptr)
	{
		// If we have an existing pawn, just use it's rotation
		SpawnRotation = NewPlayer->GetPawn()->GetActorRotation();
	}
	
	else if (AHPlayerState* PS = NewPlayer->GetPlayerState<AHPlayerState>())
	{
		
		//APawn* NewPawn = SpawnHCharacter(NewPlayer,StartSpot,TagToCharacterClassMap.FindChecked(PS->HeroTag));
		//UE_LOG(LogTemp,Warning,TEXT("Hero Tag On Game Mode = %s"),*PS->HeroTag.ToString());

		AHPlayerController* PC = Cast<AHPlayerController>(NewPlayer);

		AHCharacter* NewPawn = SpawnHCharacter(NewPlayer,StartSpot,TagToCharacterClassMap.FindChecked(PC->HeroTag));
		NewPawn->bIsRadiant = PC->bIsRadiant;
		UE_LOG(LogTemp,Warning,TEXT("Hero Tag On Game Mode = %s"),*PC->HeroTag.ToString());
		
		if (IsValid(NewPawn))
		{
			NewPlayer->SetPawn(NewPawn);
		}
	}
	else
	{
		HLOG("GameModeda Sorun var PS valid değil")
	}
	
	if (!IsValid(NewPlayer->GetPawn()))
	{
		FailedToRestartPlayer(NewPlayer);
	}
	else
	{
		// Tell the start spot it was used
		InitStartSpot(StartSpot, NewPlayer);

		FinishRestartPlayer(NewPlayer, SpawnRotation); // Dokunmaya gerek yok
	}
}

AActor* AHGameModeBase::FindHPlayerStartForTeam(bool bRadiant)
{
	UWorld* World = GetWorld();

	AActor* BestStart = nullptr;

	bool bfound = false;

	const FName TeamTag = FName(bRadiant ? TEXT("Radiant") : TEXT("Dire"));
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* Start = *It;
		if (Start && Start->PlayerStartTag == TeamTag && !Start->ActorHasTag(FName("Used")))
		{
			Start->Tags.Add(FName("Used"));
			BestStart = Start;
			bfound = true;
			break;
		}
	}

	if (bfound == false)
	{
		for (TActorIterator<APlayerStart> It(World); It; ++It)
		{
			APlayerStart* Start = *It;
			if (Start && Start->PlayerStartTag == TeamTag)
			{
				BestStart = Start;
				break;
			}
		}
	}
	
	if (BestStart == nullptr)
	{
		// No player start found
		UE_LOG(LogGameMode, Log, TEXT("FindPlayerStart: PATHS NOT DEFINED or NO PLAYERSTART with positive rating"));

		// This is a bit odd, but there was a complex chunk of code that in the end always resulted in this, so we may as well just 
		// short cut it down to this.  Basically we are saying spawn at 0,0,0 if we didn't find a proper player start
		BestStart = World->GetWorldSettings();
	}

	return BestStart;
}

AActor* AHGameModeBase::FindHPlayerRespawnLocationForTeam(bool bRadiant)
{
	UWorld* World = GetWorld();

	AActor* BestStart = nullptr;

	const FName TeamTag = FName(bRadiant ? TEXT("Radiant") : TEXT("Dire"));
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* Start = *It;
		if (Start && Start->ActorHasTag(TeamTag))
		{
			BestStart = Start;
			break;
		}
	}

	return BestStart;
}

AHCharacter* AHGameModeBase::SpawnHCharacter(AController* NewPlayer, AActor* StartSpot,
                                             TSubclassOf<AHCharacter> CharacterClass)
{
	
	// Don't allow pawn to be spawned with any pitch or roll
	FRotator StartRotation(ForceInit);
	StartRotation.Yaw = StartSpot->GetActorRotation().Yaw;
	FVector StartLocation = StartSpot->GetActorLocation();

	FTransform Transform = FTransform(StartRotation, StartLocation);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save default player pawns into a map
	
	AHCharacter* ResultPawn = GetWorld()->SpawnActor<AHCharacter>(CharacterClass, Transform, SpawnInfo);
	if (!ResultPawn)
	{
		UE_LOG(LogGameMode, Warning, TEXT("SpawnDefaultPawnAtTransform: Couldn't spawn Pawn of type %s at %s"), *GetNameSafe(CharacterClass), *Transform.ToHumanReadableString());
	}
	return ResultPawn;
}

void AHGameModeBase::I_KillEvent(AActor* KillerActor, AActor* KilledActor)
{
	KillEvent(KillerActor,KilledActor);
}

float AHGameModeBase::I_GetGameTime()
{
	return GameTime;
}

void AHGameModeBase::KillEvent(AActor* KillerActor, AActor* KilledActor)
{
	// Öldüren Oyuncu Değil - Ölen Oyuncu değil -- Hiç bişi yapma

	// Öldüren Oyuncu değil - Ölen Oyuncu ------------ Ölen oyuncu ölüm işlemi 
	if (!KillerActor->ActorHasTag(FName("Player")) && KilledActor->ActorHasTag(FName("Player")))
	{
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(KilledActor);
		if (ASC)
		{
			FGameplayTagContainer HTagContainer;
			HTagContainer.AddTag(FHGamePlayTags::Get().EffectTag_RedBuff);
			HTagContainer.AddTag(FHGamePlayTags::Get().EffectTag_BlueBuff);
			ASC->RemoveActiveEffectsWithGrantedTags(HTagContainer);

			ASC->RemoveActiveEffectsWithTags(FHGamePlayTags::Get().EffectTag_RemoveOnDeath.GetSingleTagContainer());
		}

		if (ACharacter* KilledCharacter = Cast<ACharacter>(KilledActor))
		{
			APlayerState* PS = KilledCharacter->GetPlayerState();
			check(Cast<UObject>(PS));
			IPlayerInterface::Execute_I_AddToDeathCount(Cast<UObject>(PS));
		}
		
		StartReSpawnPlayerCharacter(KilledActor);
	}
	
	// Öldüren Oyuncu --  Ölen Oyuncu değil ---- Öldüren oyuncu LastHit Arttır.
	else if (KillerActor->ActorHasTag(FName("Player")) && !KilledActor->ActorHasTag(FName("Player")))
	{
		if (ACharacter* KillerCharacter = Cast<ACharacter>(KillerActor))
		{
			APlayerState* PS = KillerCharacter->GetPlayerState();
			check(Cast<UObject>(PS));
			IPlayerInterface::Execute_I_AddToLastHitCount(Cast<UObject>(PS));
		}
	}

	// Öldüren Oyuncu - Ölen Oyuncu -- Öldüren Kill Count Arttır -- Ölen Death Count Arttır Ve Ölüm İşlemi 
	else if (KillerActor->ActorHasTag(FName("Player")) && KilledActor->ActorHasTag(FName("Player")))
	{
		if (ACharacter* KillerCharacter = Cast<ACharacter>(KillerActor))
		{
			APlayerState* PS = KillerCharacter->GetPlayerState();
			check(Cast<UObject>(PS));
			IPlayerInterface::Execute_I_AddToKillCount(Cast<UObject>(PS));
		}

		if (ACharacter* KilledCharacter = Cast<ACharacter>(KilledActor))
		{
			APlayerState* PS = KilledCharacter->GetPlayerState();
			check(Cast<UObject>(PS));
			IPlayerInterface::Execute_I_AddToDeathCount(Cast<UObject>(PS));
		}

		for (auto Element :GetActivePlayerControllers())
		{
			IPlayerInterface::Execute_I_ShowKillWidget(Element,ICombatInterface::Execute_GetHeroTag(KillerActor),ICombatInterface::Execute_GetHeroTag(KilledActor));
		}

		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(KilledActor);
		if (ASC)
		{
			FGameplayTagContainer HTagContainer;
			HTagContainer.AddTag(FHGamePlayTags::Get().EffectTag_RedBuff);
			HTagContainer.AddTag(FHGamePlayTags::Get().EffectTag_BlueBuff);
			ASC->RemoveActiveEffectsWithGrantedTags(HTagContainer);

			ASC->RemoveActiveEffectsWithTags(FHGamePlayTags::Get().EffectTag_RemoveOnDeath.GetSingleTagContainer());
		}

		StartReSpawnPlayerCharacter(KilledActor);
	}
	
}

float AHGameModeBase::GetPlayerReSpawnCooldown() const
{
	//Geliştirme iptal 8 saniye statik kalacak.
	return 8.f;
}

void AHGameModeBase::StartReSpawnPlayerCharacter(AActor* KilledActor)
{
	FTimerHandle RespawnTimerHandle;
	FTimerDelegate RespawnDelegate;
	
	RespawnDelegate.BindUFunction(this, "ReSpawnPlayerCharacter", KilledActor);
    
	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle,    
		RespawnDelegate,       
		GetPlayerReSpawnCooldown(),                 
		false,               
		-1.0f               
	);

}

void AHGameModeBase::ReSpawnPlayerCharacter(AActor* KilledActor)
{
	if (ICombatInterface* IC = Cast<ICombatInterface>(KilledActor))
	{
		AActor* RPlayerStart = FindHPlayerRespawnLocationForTeam(ICombatInterface::Execute_IsRadiant(KilledActor));
		if (RPlayerStart)
		{
			IC->ReSpawn(RPlayerStart->GetActorLocation(),RPlayerStart->GetActorRotation());
		}
		else
		{
			IC->ReSpawn(FVector::ZeroVector, FRotator::ZeroRotator);
		}
		
	}
}








