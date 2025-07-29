
#include "TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "HGASTemplate/HGASTemplate.h"
#include "HGASTemplate/Interfaces/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	//Super::Activate(); Sadece log ekliyor o yüzden çağırmaya gerek yok

	bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	//bool bHasAuthority = Ability->GetCurrentActorInfo()->IsNetAuthority();

	if(bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		//  we are on server so listen for target data
		// Serverdeyiz dedik çünkü SendMouseCursorData() da  özel tanımlı server rpc yolluyoruz yani bu kısım serverde çalışacaktır.

		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		//FPredictionKey ActivationPredictionKey = FPredictionKey();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,ActivationPredictionKey).AddUObject(this,
			&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle,ActivationPredictionKey)

		// Yukarıda callback bağladık delegateye ama ya callback bağlanmadan önce  ServerSetReplicatedTargetData() rpc si ile giden
		// bilgi önce gidip delegateyi broadcast ettirmiş işe, delegateyi callbacka bağladıktan sonra broadcast işlemi olmayacak demektir
		// bunu aşmak için aşağıdaki fonk kullanıyoruz
		const bool bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		// So if it's already been set and we're broadcasting it well, we don't have to care about anything anymore.
		// if not be called delegate, then we need to call set waiting on remote player data
		if(!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
		
	}
	
}

void UTargetDataUnderMouse::SendMouseCursorData() // Localy controlled makinede çağırılacak
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get()); // Bu fonk içindeki bloklar predict edildi
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Target, false, CursorHit);
	
	AActor* TargetActor = IPlayerInterface::Execute_GetTargetActor(PC);
	

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.AddUnique(TargetActor);
	Data->SetActors(Actors);
	
	DataHandle.Add(Data);
	

	// Servere DataHandle ı iletiyor ve FAbilityTargetDataSetDelegate e ye bu data handle ı broadcast ettiriyor
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),DataHandle,FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// Localde ability taskın delegatesini çağıralım
	if(ShouldBroadcastAbilityTaskDelegates()) // This should be called prior to broadcasting delegates back into the ability graph. This makes sure the ability is still active.
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,FGameplayTag ActivationTag)
{
	//Okay, target data has been received, don't keep it stored.
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	
	if(ShouldBroadcastAbilityTaskDelegates()) 
	{
		ValidData.Broadcast(DataHandle);
	}
}



