// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_SendEventForPlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "HGASTemplate/Characters/HCharacter.h"
#include "HGASTemplate/PlayerCore/PlayerStates/HPlayerState.h"

void UAN_SendEventForPlayerCharacter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if(MeshComp->GetOwner())
	{
		//AHCharacter* HCharacter = Cast<AHCharacter>(MeshComp->GetOwner());
		//AHPlayerState* HPlayerState = HCharacter->GetPlayerState<AHPlayerState>();
		FGameplayEventData Data;
		//UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HPlayerState,TagToSend,Data);
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(),TagToSend,Data);
	}
}
