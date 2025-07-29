// Fill out your copyright notice in the Description page of Project Settings.


#include "HAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "HGASTemplate/GamePlayTags/HGameplayTags.h"

UHAssetManager& UHAssetManager::Get()
{
	check(GEngine);
	UHAssetManager* HAssetManager = Cast<UHAssetManager>(GEngine->AssetManager);
	return *HAssetManager;
}

/**
* This is called very early on and this is when we start the initial loading for our assets for the game.
*/
void UHAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FHGamePlayTags::InitializeNativeGameplayTags(); // Oyunun başlangıcında nativetagler ekleniyor
	UAbilitySystemGlobals::Get().InitGlobalData(); // This is Required to use Target Data on Ability Task
	
}
