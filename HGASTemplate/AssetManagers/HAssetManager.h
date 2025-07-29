// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "HAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class HGASTEMPLATE_API UHAssetManager : public UAssetManager
{
	GENERATED_BODY()
	

	/**
	 * COURSE NOTES
	 * Etkinleştirmek için DefaultEngine inide işlem yapmak gerekiyor.
	* [/Script/Engine.Engine]
	+ActiveGameNameRedirects=(OldGameName="TP_BlankBP",NewGameName="/Script/Aura")
	+ActiveGameNameRedirects=(OldGameName="/Script/TP_BlankBP",NewGameName="/Script/Aura")
	AssetManagerClassName=/Script/Aura.AuraAssetManager Bu satırın eklenmesi gerekiyor
	 */

public:

	static UHAssetManager& Get();

protected:
	
	/**
	 * This is called very early on and this is when we start the initial loading for our assets for the game.
	 */
	virtual void StartInitialLoading() override;
	
};
